#include "fetchplug.h"

RelayContext relayCtx;
EnergyContext energyCtx;


// ─── Init Relay Client ───────────────────────────────────────────────
void initRelayClient() {
    relayCtx.client.onConnect([](void* arg, AsyncClient* c) {
        char request[128];
        snprintf(request, sizeof(request),
            "GET /cm?cmnd=Status%%2011 HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n\r\n",
            PUMP_IP);
        c->write(request);
    }, nullptr);

    relayCtx.client.onData([](void* arg, AsyncClient* c, void* data, size_t len) {
        if (relayCtx.len + len >= sizeof(relayCtx.buffer) - 1) return;
        memcpy(relayCtx.buffer + relayCtx.len, data, len);
        relayCtx.len += len;
        relayCtx.buffer[relayCtx.len] = '\0';

        char* jsonStart = strchr(relayCtx.buffer, '{');
        char* jsonEnd   = strrchr(relayCtx.buffer, '}');
        if (jsonStart && jsonEnd && jsonEnd > jsonStart) {
            StaticJsonDocument<256> doc;
            if (deserializeJson(doc, jsonStart) == DeserializationError::Ok) {
                const char* state = doc["StatusSTS"]["POWER"];
                if (state) pump_running = (strcmp(state, "ON") == 0) ? 1 : 0;
            }
        }
    }, nullptr);

    relayCtx.client.onDisconnect([](void* arg, AsyncClient* c) {
        relayCtx.requestInProgress = false;
        relayCtx.len = 0;
        relayCtx.readyForNext = true;
    }, nullptr);

    relayCtx.client.onError([](void* arg, AsyncClient* c, int8_t error) {
        relayCtx.requestInProgress = false;
        relayCtx.len = 0;
        relayCtx.retryAfter = millis() + 2000;
    }, nullptr);

    relayCtx.initialized = true;
}

// ─── Init Energy Client ──────────────────────────────────────────────
void initEnergyClient() {
    energyCtx.client.onConnect([](void* arg, AsyncClient* c) {
        char request[128];
        snprintf(request, sizeof(request),
            "GET /cm?cmnd=Status%%2010 HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n\r\n",
            PUMP_IP);
        c->write(request);
    }, nullptr);

    energyCtx.client.onData([](void* arg, AsyncClient* c, void* data, size_t len) {
        if (energyCtx.len + len >= sizeof(energyCtx.buffer) - 1) return;
        memcpy(energyCtx.buffer + energyCtx.len, data, len);
        energyCtx.len += len;
        energyCtx.buffer[energyCtx.len] = '\0';

        char* jsonStart = strchr(energyCtx.buffer, '{');
        char* jsonEnd   = strrchr(energyCtx.buffer, '}');
        if (jsonStart && jsonEnd && jsonEnd > jsonStart) {
            StaticJsonDocument<768> doc;
            if (deserializeJson(doc, jsonStart) == DeserializationError::Ok) {
                JsonObject energy = doc["StatusSNS"]["ENERGY"];
                if (!energy.isNull()) {
                    power          = energy["Power"]         | 0.0f;
                    voltage        = energy["Voltage"]       | 0.0f;
                    current        = energy["Current"]       | 0.0f;
                    ApparentPower  = energy["ApparentPower"] | 0.0f;
                    ReactivePower  = energy["ReactivePower"] | 0.0f;
                    PowerFactor    = energy["Factor"]        | 0.0f;
                    TotalEnergy    = energy["Total"]         | 0.0f;
                    yesterdayEnergy= energy["Yesterday"]     | 0.0f;
                    todayEnergy    = energy["Today"]         | 0.0f;
                }
            }
        }
    }, nullptr);

    energyCtx.client.onDisconnect([](void* arg, AsyncClient* c) {
        energyCtx.requestInProgress = false;
        energyCtx.len = 0;
        energyCtx.readyForNext = true;
    }, nullptr);

    energyCtx.client.onError([](void* arg, AsyncClient* c, int8_t error) {
        energyCtx.requestInProgress = false;
        energyCtx.len = 0;
        energyCtx.retryAfter = millis() + 2000;
    }, nullptr);

    energyCtx.initialized = true;
}

// ─── Public fetch function ───────────────────────────────────────────
void fetchplug() {
    if (WiFi.status() != WL_CONNECTED) return;
    if (!relayCtx.initialized) initRelayClient();
    relayCtx.requestInProgress = true;
    relayCtx.len = 0;
    relayCtx.client.connect(PUMP_IP, PLUG_PORT);
}

void fetchplugEnergy() {
    if (WiFi.status() != WL_CONNECTED) return;
    if (!energyCtx.initialized) initEnergyClient();
    energyCtx.requestInProgress = true;
    energyCtx.len = 0;
    energyCtx.client.connect(PUMP_IP, PLUG_PORT);
}