#include "fetchtank.h"

TankContext tankCtx;


void initTankClient() {
    tankCtx.client.onConnect([](void* arg, AsyncClient* c) {
        char request[128];
        snprintf(request, sizeof(request),
            "GET /cm?cmnd=Status%%2010 HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n\r\n",
            TANK_IP);
        c->write(request);
    }, nullptr);

    tankCtx.client.onData([](void* arg, AsyncClient* c, void* data, size_t len) {
        if (tankCtx.len + len >= sizeof(tankCtx.buffer) - 1) return;
        memcpy(tankCtx.buffer + tankCtx.len, data, len);
        tankCtx.len += len;
        tankCtx.buffer[tankCtx.len] = '\0';

        char* jsonStart = strchr(tankCtx.buffer, '{');
        char* jsonEnd   = strrchr(tankCtx.buffer, '}');
        if (jsonStart && jsonEnd && jsonEnd > jsonStart) {
            StaticJsonDocument<768> doc;
            if (deserializeJson(doc, jsonStart) == DeserializationError::Ok) {

                // ── Flow rate ──────────────────────────────────────
                uint32_t newCount = doc["StatusSNS"]["COUNTER"]["C1"] | 0;
                static uint32_t prevCount = 0;
                if (newCount >= prevCount) {  // guard against counter reset
                    currentLPM = (float)(newCount - prevCount) * 60.0f / 56.0f;
                }
                prevCount = newCount;

                // ── Water level switches ───────────────────────────
                char key[12];
                for (int i = 0; i < 8; i++) {
                    snprintf(key, sizeof(key), "Switch%d", i + 1);
                    const char* state = doc["StatusSNS"][key] | "ON";
                    waterLevel[i] = (strcmp(state, "OFF") == 0) ? 0 : 1;
                }
                tank_full = waterLevel[7];
            }
        }
    }, nullptr);

    tankCtx.client.onDisconnect([](void* arg, AsyncClient* c) {
        tankCtx.requestInProgress = false;
        tankCtx.len = 0;
        tankCtx.readyForNext = true;
    }, nullptr);

    tankCtx.client.onError([](void* arg, AsyncClient* c, int8_t error) {
        tankCtx.requestInProgress = false;
        tankCtx.len = 0;
        tankCtx.retryAfter = millis() + 2000;
    }, nullptr);

    tankCtx.initialized = true;
}

void fetchtank() {
    if (WiFi.status() != WL_CONNECTED) return;
    if (!tankCtx.initialized) initTankClient();
    tankCtx.requestInProgress = true;
    tankCtx.len = 0;
    tankCtx.client.connect(TANK_IP, 80);
}