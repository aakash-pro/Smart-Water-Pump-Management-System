
#include "pumpcontrol.h"


void turnOnPumpAsync() {
    static bool requestInProgress = false;
    if (requestInProgress) return;
    if (WiFi.status() != WL_CONNECTED)
        return;

    requestInProgress = true;
    String hostRelay = String(PUMP_IP);
    uint16_t port = 80;
    String requestRelay = String("GET /cm?cmnd=Power%20ON HTTP/1.1\r\nHost: ") + hostRelay + "\r\nConnection: close\r\n\r\n";

    struct ClientContext {
        AsyncClient* client;
        String responseBuffer;
        bool isClosed = false;
    };
    ClientContext* ctx = new ClientContext();
    ctx->client = new AsyncClient();

    ctx->client->onConnect([ctx, requestRelay](void* arg, AsyncClient* c) {
        if (c->space() > requestRelay.length()) {
            c->write(requestRelay.c_str());
        }
    }, nullptr);

    ctx->client->onData([ctx](void* arg, AsyncClient* c, void* data, size_t len) {
        ctx->responseBuffer += String((char*)data).substring(0, len);
        int jsonStart = ctx->responseBuffer.indexOf('{');
        int jsonEnd = ctx->responseBuffer.lastIndexOf('}');
        if (jsonStart >= 0 && jsonEnd > jsonStart) {
            String jsonStr = ctx->responseBuffer.substring(jsonStart, jsonEnd + 1);
            StaticJsonDocument<256> doc;
            DeserializationError err = deserializeJson(doc, jsonStr);
            if (!err) {
                String relayState = doc["POWER"] | String("");
                // Optionally update pump_running or log result here
            }
        }
    }, nullptr);

    ctx->client->onError([ctx](void* arg, AsyncClient* c, int8_t error) {
        if (!ctx->isClosed) {
            ctx->isClosed = true;
            c->close();
            delete ctx->client;
            delete ctx;
            requestInProgress = false;
        }
    }, nullptr);

    ctx->client->onDisconnect([ctx](void* arg, AsyncClient* c) {
        if (!ctx->isClosed) {
            ctx->isClosed = true;
            delete ctx->client;
            delete ctx;
            requestInProgress = false;
        }
    }, nullptr);

    if (!ctx->client->connect(hostRelay.c_str(), port)) {
        delete ctx->client;
        delete ctx;
        requestInProgress = false;
    }
}

void turnOffPumpAsync() {
    static bool requestInProgress = false;
    if (requestInProgress) return;
    if (WiFi.status() != WL_CONNECTED)
        return;

    requestInProgress = true;
    String hostRelay = String(PUMP_IP);
    uint16_t port = 80;
    String requestRelay = String("GET /cm?cmnd=Power%20OFF HTTP/1.1\r\nHost: ") + hostRelay + "\r\nConnection: close\r\n\r\n";

    struct ClientContext {
        AsyncClient* client;
        String responseBuffer;
        bool isClosed = false;
    };
    ClientContext* ctx = new ClientContext();
    ctx->client = new AsyncClient();

    ctx->client->onConnect([ctx, requestRelay](void* arg, AsyncClient* c) {
        if (c->space() > requestRelay.length()) {
            c->write(requestRelay.c_str());
        }
    }, nullptr);

    ctx->client->onData([ctx](void* arg, AsyncClient* c, void* data, size_t len) {
        ctx->responseBuffer += String((char*)data).substring(0, len);
        int jsonStart = ctx->responseBuffer.indexOf('{');
        int jsonEnd = ctx->responseBuffer.lastIndexOf('}');
        if (jsonStart >= 0 && jsonEnd > jsonStart) {
            String jsonStr = ctx->responseBuffer.substring(jsonStart, jsonEnd + 1);
            StaticJsonDocument<256> doc;
            DeserializationError err = deserializeJson(doc, jsonStr);
            if (!err) {
                String relayState = doc["POWER"] | String("");
                // Optionally update pump_running or log result here
            }
        }
    }, nullptr);

    ctx->client->onError([ctx](void* arg, AsyncClient* c, int8_t error) {
        if (!ctx->isClosed) {
            ctx->isClosed = true;
            c->close();
            delete ctx->client;
            delete ctx;
            requestInProgress = false;
        }
    }, nullptr);

    ctx->client->onDisconnect([ctx](void* arg, AsyncClient* c) {
        if (!ctx->isClosed) {
            ctx->isClosed = true;
            delete ctx->client;
            delete ctx;
            requestInProgress = false;
        }
    }, nullptr);

    if (!ctx->client->connect(hostRelay.c_str(), port)) {
        delete ctx->client;
        delete ctx;
        requestInProgress = false;
    }
}
