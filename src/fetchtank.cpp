#include "fetchtank.h"


struct TankClientContext {
  AsyncClient* client;
  String responseBuffer;
  bool isClosed = false;
};


void fetchtank() {
  static bool requestInProgress = false;
  if (requestInProgress) return;
  if (WiFi.status() != WL_CONNECTED)
    return;

  requestInProgress = true;
  String hostTank = String(TANK_IP);
  uint16_t port = 80;
  String requestTank = String("GET /cm?cmnd=Status%2010 HTTP/1.1\r\nHost: ") + hostTank + "\r\nConnection: close\r\n\r\n";

  TankClientContext* ctx = new TankClientContext();
  ctx->client = new AsyncClient();

  ctx->client->onConnect([ctx, requestTank](void* arg, AsyncClient* c) {
    if (c->space() > requestTank.length()) {
      c->write(requestTank.c_str());
    }
  }, nullptr);

  ctx->client->onData([ctx](void* arg, AsyncClient* c, void* data, size_t len) {
    String chunk = String((char*)data).substring(0, len);
    ctx->responseBuffer += chunk; // accumulate response chunks

    int jsonStart = ctx->responseBuffer.indexOf('{');
    int jsonEnd = ctx->responseBuffer.lastIndexOf('}');
    if (jsonStart >= 0 && jsonEnd > jsonStart) {
      String jsonStr = ctx->responseBuffer.substring(jsonStart, jsonEnd + 1);
      DynamicJsonDocument doc(1024);
      DeserializationError err = deserializeJson(doc, jsonStr);
      if (!err) {
        uint32_t newCount = doc["StatusSNS"]["COUNTER"]["C1"] | 0;
        static uint32_t prevCount = 0;
        currentLPM = (float)(newCount - prevCount) * 60.0 / 56;
        prevCount = newCount;
        for (int i = 0; i < 8; i++) {
          String key = "Switch" + String(i + 1);
          String state = doc["StatusSNS"][key] | String("ON");
          waterLevel[i] = (state == "OFF") ? 0 : 1;
        }
        tank_full = waterLevel[7];
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

  if (!ctx->client->connect(hostTank.c_str(), port)) {
    delete ctx->client;
    delete ctx;
    requestInProgress = false;
  }
}
