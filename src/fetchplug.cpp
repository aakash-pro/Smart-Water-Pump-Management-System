#include "fetchplug.h"

struct ClientContext {
  AsyncClient* client;
  String responseBuffer;
  bool isClosed = false;
};


void fetchplug() {
  static bool relayRequestInProgress = false;
  static bool energyRequestInProgress = false;

  if (WiFi.status() != WL_CONNECTED)
    return;

  String hostRelay = String(PUMP_IP);
  uint16_t port = 80;

  // 1. Get relay status (Status 11)
  if (!relayRequestInProgress) {
    relayRequestInProgress = true;
    String requestRelay = String("GET /cm?cmnd=Status%2011 HTTP/1.1\r\nHost: ") + hostRelay + "\r\nConnection: close\r\n\r\n";
    ClientContext* relayCtx = new ClientContext();
    relayCtx->client = new AsyncClient();

    relayCtx->client->onConnect([relayCtx, requestRelay](void* arg, AsyncClient* c) {
      if (c->space() > requestRelay.length()) {
        c->write(requestRelay.c_str());
      }
    }, nullptr);

    relayCtx->client->onData([relayCtx](void* arg, AsyncClient* c, void* data, size_t len) {
      relayCtx->responseBuffer += String((char*)data).substring(0, len);
      int jsonStart = relayCtx->responseBuffer.indexOf('{');
      int jsonEnd = relayCtx->responseBuffer.lastIndexOf('}');
      if (jsonStart >= 0 && jsonEnd > jsonStart) {
        String jsonStr = relayCtx->responseBuffer.substring(jsonStart, jsonEnd + 1);
        StaticJsonDocument<256> doc;
        DeserializationError err = deserializeJson(doc, jsonStr);
        if (!err) {
          String relayState = doc["StatusSTS"]["POWER"] | String("");
          pump_running = (relayState == "ON") ? 1 : 0;
        }
      }
    }, nullptr);

    relayCtx->client->onError([relayCtx](void* arg, AsyncClient* c, int8_t error) {
      if (!relayCtx->isClosed) {
        relayCtx->isClosed = true;
        c->close();
        delete relayCtx->client;
        delete relayCtx;
        relayRequestInProgress = false;
      }
    }, nullptr);

    relayCtx->client->onDisconnect([relayCtx](void* arg, AsyncClient* c) {
      if (!relayCtx->isClosed) {
        relayCtx->isClosed = true;
        delete relayCtx->client;
        delete relayCtx;
        relayRequestInProgress = false;
      }
    }, nullptr);

    if (!relayCtx->client->connect(hostRelay.c_str(), port)) {
      delete relayCtx->client;
      delete relayCtx;
      relayRequestInProgress = false;
    }
  }

  // 2. Get energy parameters (Status 10)
  if (!energyRequestInProgress) {
    energyRequestInProgress = true;
    String requestEnergy = String("GET /cm?cmnd=Status%2010 HTTP/1.1\r\nHost: ") + hostRelay + "\r\nConnection: close\r\n\r\n";
    ClientContext* energyCtx = new ClientContext();
    energyCtx->client = new AsyncClient();

    energyCtx->client->onConnect([energyCtx, requestEnergy](void* arg, AsyncClient* c) {
      if (c->space() > requestEnergy.length()) {
        c->write(requestEnergy.c_str());
      }
    }, nullptr);

    energyCtx->client->onData([energyCtx](void* arg, AsyncClient* c, void* data, size_t len) {
      energyCtx->responseBuffer += String((char*)data).substring(0, len);
      int jsonStart = energyCtx->responseBuffer.indexOf('{');
      int jsonEnd = energyCtx->responseBuffer.lastIndexOf('}');
      if (jsonStart >= 0 && jsonEnd > jsonStart) {
        String jsonStr = energyCtx->responseBuffer.substring(jsonStart, jsonEnd + 1);
        StaticJsonDocument<512> doc;
        DeserializationError err = deserializeJson(doc, jsonStr);
        if (!err) {
          JsonObject energy = doc["StatusSNS"]["ENERGY"];
          power = energy["Power"] | 0.0;
          voltage = energy["Voltage"] | 0.0;
          current = energy["Current"] | 0.0;
          ApparentPower = energy["ApparentPower"] | 0.0;
          ReactivePower = energy["ReactivePower"] | 0.0;
          PowerFactor = energy["Factor"] | 0.0;
          TotalEnergy = energy["Total"] | 0.0;
          yesterdayEnergy = energy["Yesterday"] | 0.0;
          todayEnergy = energy["Today"] | 0.0;
        }
      }
    }, nullptr);

    energyCtx->client->onError([energyCtx](void* arg, AsyncClient* c, int8_t error) {
      if (!energyCtx->isClosed) {
        energyCtx->isClosed = true;
        c->close();
        delete energyCtx->client;
        delete energyCtx;
        energyRequestInProgress = false;
      }
    }, nullptr);

    energyCtx->client->onDisconnect([energyCtx](void* arg, AsyncClient* c) {
      if (!energyCtx->isClosed) {
        energyCtx->isClosed = true;
        delete energyCtx->client;
        delete energyCtx;
        energyRequestInProgress = false;
      }
    }, nullptr);

    if (!energyCtx->client->connect(hostRelay.c_str(), port)) {
      delete energyCtx->client;
      delete energyCtx;
      energyRequestInProgress = false;
    }
  }
}
