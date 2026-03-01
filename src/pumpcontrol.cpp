
#include "pumpcontrol.h"
#define RELAY_PORT 80

static AsyncClient pumpClient;
PumpContext pumpCtx;

void initPumpClient() {
    pumpClient.onConnect([](void* arg, AsyncClient* client) {
        PumpContext* ctx = (PumpContext*)arg;
        char request[128];
        snprintf(request, sizeof(request),
            "GET /cm?cmnd=Power%%20%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n\r\n",
            ctx->command, PUMP_IP);
        client->write(request);
    }, &pumpCtx);

    pumpClient.onData([](void* arg, AsyncClient* client, void* data, size_t len) {
        PumpContext* ctx = (PumpContext*)arg;
        if (ctx->len + len >= sizeof(ctx->buffer) - 1) return;
        memcpy(ctx->buffer + ctx->len, data, len);
        ctx->len += len;
        ctx->buffer[ctx->len] = '\0';

        char* jsonStart = strchr(ctx->buffer, '{');
        char* jsonEnd   = strrchr(ctx->buffer, '}');
        if (jsonStart && jsonEnd && jsonEnd > jsonStart) {
            StaticJsonDocument<128> doc;
            if (deserializeJson(doc, jsonStart) == DeserializationError::Ok) {
                const char* state = doc["POWER"] | "";
                if (*state) {  // Check if not empty
                    strncpy(ctx->lastKnownState, state, sizeof(ctx->lastKnownState) - 1);
                    ctx->responseReceived = true;
                    
                }
            }
        }
    }, &pumpCtx);

    pumpClient.onDisconnect([](void* arg, AsyncClient* client) {
        PumpContext* ctx = (PumpContext*)arg;
        ctx->requestInProgress = false;
        ctx->len = 0;
    }, &pumpCtx);

    pumpClient.onError([](void* arg, AsyncClient* client, int8_t error) {
        PumpContext* ctx = (PumpContext*)arg;
        ctx->requestInProgress = false;
        ctx->retryAfter = millis() + 2000;
    }, &pumpCtx);
}

void pumpRequest(const char* command) {
    static bool initialized = false;
    if (!initialized) {
        initPumpClient();
        initialized = true;
    }
    if (pumpCtx.requestInProgress || WiFi.status() != WL_CONNECTED) return;
    strncpy(pumpCtx.command, command, sizeof(pumpCtx.command) - 1);
    pumpCtx.command[sizeof(pumpCtx.command) - 1] = '\0';
    pumpCtx.requestInProgress = true;
    pumpCtx.responseReceived = false;
    pumpCtx.len = 0;
    memset(pumpCtx.buffer, 0, sizeof(pumpCtx.buffer));  // Clear buffer before new request
    pumpClient.connect(PUMP_IP, RELAY_PORT);
}


void turnOnPumpAsync()
{
    pumpRequest("ON");
}


void turnOffPumpAsync()
{
    pumpRequest("OFF");
}
