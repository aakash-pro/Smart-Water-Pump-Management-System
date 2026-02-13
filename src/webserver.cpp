#include "webserver.h"
#include "libraries.h"
#include "hardware.h"
#include "data.h"



ESP8266WebServer server(80);

void initWebServer() {
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    return;
  }
  // Diagnostic: list expected files and sizes
  const char *files[] = {"/index.html", "/style.css", "/app.js"};
  for (size_t i = 0; i < sizeof(files)/sizeof(files[0]); ++i) {
    const char *p = files[i];
    bool ex = LittleFS.exists(p);
    (void)p; (void)ex;
    if (ex) {
      File f = LittleFS.open(p, "r");
      if (f) {
        f.close();
      }
    }
  }
  
  // Serve index.html
  server.on("/", HTTP_GET, [](void) {
    if (LittleFS.exists("/index.html")) {
      File file = LittleFS.open("/index.html", "r");
      server.streamFile(file, "text/html");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    }
  });

  // Serve CSS
  server.on("/style.css", HTTP_GET, [](void) {
    if (LittleFS.exists("/style.css")) {
      File file = LittleFS.open("/style.css", "r");
      server.streamFile(file, "text/css");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    }
  });

  // Serve JS
  server.on("/app.js", HTTP_GET, [](void) {
    if (LittleFS.exists("/app.js")) {
      File file = LittleFS.open("/app.js", "r");
      server.streamFile(file, "application/javascript");
      file.close();
    } else {
      server.send(404, "text/plain", "File not found");
    }
  });

  // API: GET all data as JSON
  server.on("/api/data", HTTP_GET, [](void) {
    DynamicJsonDocument doc(2048);
    
    // Water/tank data
    doc["waterLevel"] = waterLevel;
    doc["tank_full"] = tank_full;
    doc["currentLPM"] = currentLPM;
    
    // Pump data
    doc["pump_running"] = pump_running;
    doc["power"] = power;
    doc["voltage"] = voltage;
    doc["current"] = current;
    doc["ApparentPower"] = ApparentPower;
    doc["ReactivePower"] = ReactivePower;
    doc["PowerFactor"] = PowerFactor;
    doc["TotalEnergy"] = TotalEnergy;
    doc["yesterdayEnergy"] = yesterdayEnergy;
    doc["todayEnergy"] = todayEnergy;
    
    // Settings
    doc["dry_run_cutoff_protection"] = dry_run_cutoff_protection;
    doc["dry_run_cutoff_power_1"] = dry_run_cutoff_power_1;
    doc["dry_run_cutoff_power_2"] = dry_run_cutoff_power_2;
    doc["dry_run_cutoff_power_3"] = dry_run_cutoff_power_3;
    doc["dry_run_cutoff_power_4"] = dry_run_cutoff_power_4;
    doc["dry_run_cutoff_delay"] = dry_run_cutoff_delay;
    doc["tank_full_cutoff_protection"] = tank_full_cutoff_protection;
    doc["tank_full_cutoff_level"] = tank_full_cutoff_level;
    doc["tank_full_cutoff_delay"] = tank_full_cutoff_delay;
    doc["dashboard_style"] = dashboard_style;
    doc["led_strip_style"] = led_strip_style;
    
    String json;
    serializeJson(doc, json);
    server.send(200, "application/json", json);
  });

  // API: POST to update a variable
  server.on("/api/variable", HTTP_POST, [](void) {
    if (server.hasArg("name") && server.hasArg("value")) {
      String name = server.arg("name");
      uint8_t value = server.arg("value").toInt();
      
      if (name == "dry_run_cutoff_protection") dry_run_cutoff_protection = value;
      else if (name == "dry_run_cutoff_power_1") dry_run_cutoff_power_1 = value;
      else if (name == "dry_run_cutoff_power_2") dry_run_cutoff_power_2 = value;
      else if (name == "dry_run_cutoff_power_3") dry_run_cutoff_power_3 = value;
      else if (name == "dry_run_cutoff_power_4") dry_run_cutoff_power_4 = value;
      else if (name == "dry_run_cutoff_delay") dry_run_cutoff_delay = value;
      else if (name == "tank_full_cutoff_protection") tank_full_cutoff_protection = value;
      else if (name == "tank_full_cutoff_level") tank_full_cutoff_level = value;
      else if (name == "tank_full_cutoff_delay") tank_full_cutoff_delay = value;
      else if (name == "dashboard_style") dashboard_style = value;
      else if (name == "led_strip_style") led_strip_style = value;
      else {
        server.send(400, "application/json", "{\"error\":\"Unknown variable\"}");
        return;
      }
      
      server.send(200, "application/json", "{\"status\":\"ok\"}");
    } else {
      server.send(400, "application/json", "{\"error\":\"Missing name or value\"}");
    }
  });

  server.onNotFound([](void) {
    server.send(404, "application/json", "{\"error\":\"Not found\"}");
  });

  server.begin();
}

void handleWebServer() {
  server.handleClient();
}
