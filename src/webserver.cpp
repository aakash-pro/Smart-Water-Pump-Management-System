#include "webserver.h"

ESP8266WebServer server(80);

// ── Helper: Uptime ───────────────────────────────────────────────────
void formatUptime(char* buf, size_t size) {
    unsigned long seconds = millis() / 1000;
    unsigned int days    = seconds / 86400; seconds %= 86400;
    unsigned int hours   = seconds / 3600;  seconds %= 3600;
    unsigned int minutes = seconds / 60;    seconds %= 60;
    snprintf(buf, size, "%ud %02uh %02um %02lus", days, hours, minutes, seconds);
}

// ── Helper: Signal Quality ───────────────────────────────────────────
int getSignalQuality(int rssi) {
    if (rssi <= -100) return 0;
    if (rssi >= -50)  return 100;
    return 2 * (rssi + 100);
}

// ── Init Web Server ──────────────────────────────────────────────────
void initWebServer() {

    // ── API: GET device info ─────────────────────────────────────────
    server.on("/api/device", HTTP_GET, []() {
        StaticJsonDocument<512> doc;
        char uptime[32];
        formatUptime(uptime, sizeof(uptime));
        doc["device_name"]  = "Water Controller";
        doc["chip_id"]      = ESP.getChipId();
        doc["flash_size"]   = ESP.getFlashChipSize();
        doc["cpu_freq_mhz"] = ESP.getCpuFreqMHz();
        doc["free_heap"]    = ESP.getFreeHeap();
        doc["sdk_version"]  = ESP.getSdkVersion();
        doc["uptime"]       = uptime;
        String json;
        serializeJson(doc, json);
        server.send(200, "application/json", json);
    });

    // ── API: GET network info ────────────────────────────────────────
    server.on("/api/network", HTTP_GET, []() {
        StaticJsonDocument<512> doc;
        doc["ssid"]           = WiFi.SSID();
        doc["ip"]             = WiFi.localIP().toString();
        doc["gateway"]        = WiFi.gatewayIP().toString();
        doc["subnet"]         = WiFi.subnetMask().toString();
        doc["mac"]            = WiFi.macAddress();
        doc["rssi"]           = WiFi.RSSI();
        doc["signal_quality"] = getSignalQuality(WiFi.RSSI());
        String json;
        serializeJson(doc, json);
        server.send(200, "application/json", json);
    });

    // ── API: GET sensor data ─────────────────────────────────────────
    server.on("/api/sensors", HTTP_GET, []() {
        StaticJsonDocument<512> doc;
        JsonArray levels = doc.createNestedArray("waterLevel");
        for (int i = 0; i < 8; i++) levels.add(waterLevel[i]);
        doc["tank_full"]      = tank_full;
        doc["pump_running"]   = pump_running;
        doc["currentLPM"]     = currentLPM;
        doc["voltage"]        = voltage;
        doc["current"]        = current;
        doc["power"]          = power;
        doc["ApparentPower"]  = ApparentPower;
        doc["ReactivePower"]  = ReactivePower;
        doc["PowerFactor"]    = PowerFactor;
        doc["TotalEnergy"]    = TotalEnergy;
        doc["todayEnergy"]    = todayEnergy;
        doc["yesterdayEnergy"]= yesterdayEnergy;
        String json;
        serializeJson(doc, json);
        server.send(200, "application/json", json);
    });

    // ── API: GET config ──────────────────────────────────────────────
    server.on("/api/config", HTTP_GET, []() {
        StaticJsonDocument<512> doc;
        doc["dry_run_cutoff_protection"]  = dry_run_cutoff_protection;
        doc["dry_run_cutoff_power_1"]     = dry_run_cutoff_power_1;
        doc["dry_run_cutoff_power_2"]     = dry_run_cutoff_power_2;
        doc["dry_run_cutoff_power_3"]     = dry_run_cutoff_power_3;
        doc["dry_run_cutoff_power_4"]     = dry_run_cutoff_power_4;
        doc["dry_run_cutoff_delay"]       = dry_run_cutoff_delay;
        doc["tank_full_cutoff_protection"]= tank_full_cutoff_protection;
        doc["tank_full_cutoff_level"]     = tank_full_cutoff_level;
        doc["tank_full_cutoff_delay"]     = tank_full_cutoff_delay;
        doc["dashboard_style"]            = dashboard_style;
        doc["led_strip_style"]            = led_strip_style;
        doc["overload_cutoff_protection"] = overload_cutoff_protection;
        doc["overload_cutoff_power_1"]    = overload_cutoff_power_1;
        doc["overload_cutoff_power_2"]    = overload_cutoff_power_2;
        doc["overload_cutoff_power_3"]    = overload_cutoff_power_3;
        doc["overload_cutoff_power_4"]    = overload_cutoff_power_4;
        doc["overload_cutoff_delay"]      = overload_cutoff_delay;
        String json;
        serializeJson(doc, json);
        server.send(200, "application/json", json);
    });

    // ── API: POST config ─────────────────────────────────────────────
    server.on("/api/config", HTTP_POST, []() {
        if (!server.hasArg("plain")) {
            server.send(400, "application/json", "{\"error\":\"No body\"}");
            return;
        }
        StaticJsonDocument<512> doc;
        DeserializationError err = deserializeJson(doc, server.arg("plain"));
        if (err) {
            server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }

        // dry run
        if (doc.containsKey("dry_run_cutoff_protection")) {
            uint8_t val = doc["dry_run_cutoff_protection"];
            if (val <= 1) dry_run_cutoff_protection = val;
        }
        if (doc.containsKey("dry_run_cutoff_power_1")) {
            uint8_t val = doc["dry_run_cutoff_power_1"];
            if (val <= 1) dry_run_cutoff_power_1 = val;      // max 1 — thousands digit
        }
        if (doc.containsKey("dry_run_cutoff_power_2")) {
            uint8_t val = doc["dry_run_cutoff_power_2"];
            if (val <= 9) dry_run_cutoff_power_2 = val;
        }
        if (doc.containsKey("dry_run_cutoff_power_3")) {
            uint8_t val = doc["dry_run_cutoff_power_3"];
            if (val <= 9) dry_run_cutoff_power_3 = val;
        }
        if (doc.containsKey("dry_run_cutoff_power_4")) {
            uint8_t val = doc["dry_run_cutoff_power_4"];
            if (val <= 9) dry_run_cutoff_power_4 = val;
        }
        if (doc.containsKey("dry_run_cutoff_delay")) {
            uint8_t val = doc["dry_run_cutoff_delay"];
            if (val <= 60) dry_run_cutoff_delay = val;
        }

        // tank full
        if (doc.containsKey("tank_full_cutoff_protection")) {
            uint8_t val = doc["tank_full_cutoff_protection"];
            if (val <= 1) tank_full_cutoff_protection = val;
        }
        if (doc.containsKey("tank_full_cutoff_level")) {
            uint8_t val = doc["tank_full_cutoff_level"];
            if (val >= 1 && val <= 8) tank_full_cutoff_level = val;
        }
        if (doc.containsKey("tank_full_cutoff_delay")) {
            uint8_t val = doc["tank_full_cutoff_delay"];
            if (val <= 200) tank_full_cutoff_delay = val;
        }

        // overload
        if (doc.containsKey("overload_cutoff_protection")) {
            uint8_t val = doc["overload_cutoff_protection"];
            if (val <= 1) overload_cutoff_protection = val;
        }
        if (doc.containsKey("overload_cutoff_power_1")) {
            uint8_t val = doc["overload_cutoff_power_1"];
            if (val <= 1) overload_cutoff_power_1 = val;     // max 1 — thousands digit
        }
        if (doc.containsKey("overload_cutoff_power_2")) {
            uint8_t val = doc["overload_cutoff_power_2"];
            if (val <= 9) overload_cutoff_power_2 = val;
        }
        if (doc.containsKey("overload_cutoff_power_3")) {
            uint8_t val = doc["overload_cutoff_power_3"];
            if (val <= 9) overload_cutoff_power_3 = val;
        }
        if (doc.containsKey("overload_cutoff_power_4")) {
            uint8_t val = doc["overload_cutoff_power_4"];
            if (val <= 9) overload_cutoff_power_4 = val;
        }
        if (doc.containsKey("overload_cutoff_delay")) {
            uint8_t val = doc["overload_cutoff_delay"];
            if (val <= 60) overload_cutoff_delay = val;
        }

        // display
        if (doc.containsKey("dashboard_style")) {
            uint8_t val = doc["dashboard_style"];
            if (val >= 1 && val <= 4) dashboard_style = val;
        }
        if (doc.containsKey("led_strip_style")) {
            uint8_t val = doc["led_strip_style"];
            if (val >= 1 && val <= 4) led_strip_style = val;
        }

        saveValuesToEEPROM();
        server.send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // ── API: GET pump state ──────────────────────────────────────────
    server.on("/api/pump", HTTP_GET, []() {
        StaticJsonDocument<64> doc;
        doc["state"]          = pump_running;
        doc["lastKnownState"] = pumpCtx.lastKnownState;
        String json;
        serializeJson(doc, json);
        server.send(200, "application/json", json);
    });

    // ── API: POST pump control ───────────────────────────────────────
    server.on("/api/pump", HTTP_POST, []() {
        if (!server.hasArg("plain")) {
            server.send(400, "application/json", "{\"error\":\"No body\"}");
            return;
        }
        StaticJsonDocument<64> doc;
        DeserializationError err = deserializeJson(doc, server.arg("plain"));
        if (err) {
            server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }
        if (!doc.containsKey("state")) {
            server.send(400, "application/json", "{\"error\":\"Missing state\"}");
            return;
        }
        bool requestedState = doc["state"];
        if (requestedState)
        { turnOnPumpAsync(); }
        else { turnOffPumpAsync(); }
        server.send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // ── API: POST system commands ────────────────────────────────────
    server.on("/api/system", HTTP_POST, []() {
        if (!server.hasArg("plain")) {
            server.send(400, "application/json", "{\"error\":\"No body\"}");
            return;
        }
        StaticJsonDocument<128> doc;
        DeserializationError err = deserializeJson(doc, server.arg("plain"));
        if (err) {
            server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }
        const char* cmd = doc["command"];
        if (!cmd) {
            server.send(400, "application/json", "{\"error\":\"Missing command\"}");
            return;
        }
        if (strcmp(cmd, "reset") == 0) {
            server.send(200, "application/json", "{\"status\":\"resetting\"}");
            reset();
            server.client().flush();
        }
        else {
            server.send(400, "application/json", "{\"error\":\"Unknown command\"}");
        }
    });

    // ── Static files + 404 — single generic handler ──────────────────
    server.onNotFound([]() {
        String path = server.uri();
        if (path == "/") path = "/index.html";

        String mime = "text/plain";
        if      (path.endsWith(".html")) mime = "text/html";
        else if (path.endsWith(".css"))  mime = "text/css";
        else if (path.endsWith(".js"))   mime = "application/javascript";
        else if (path.endsWith(".json")) mime = "application/json";
        else if (path.endsWith(".ico"))  mime = "image/x-icon";
        else if (path.endsWith(".png"))  mime = "image/png";

        if (LittleFS.exists(path)) {
            File file = LittleFS.open(path, "r");
            server.streamFile(file, mime);
            file.close();
            return;
        }
        server.send(404, "application/json", "{\"error\":\"Not found\"}");
    });

    server.begin();
}

// ── Handle client ────────────────────────────────────────────────────
void handleWebServer() {
    server.handleClient();
}