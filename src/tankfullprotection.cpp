#include "tankfullprotection.h"


void checktankfullprotection()
{  
  // Static variables to retain state between function calls
 static unsigned long tankFullDetectedTime = 0;
  static bool tankFullDelayActive = false;
  // Check if cutoff protection is enabled and water level reached cutoff
  static unsigned long lastBeepTime = 0;
  if (!tank_full_cutoff_protection && pump_running && waterLevel[tank_full_cutoff_level - 1]) {
    if (!tankFullDelayActive) {
      tankFullDetectedTime = millis();
      tankFullDelayActive = true;
    }
    // Beep every 200ms while delay is active
    if (tankFullDelayActive && (millis() - lastBeepTime >= 100)) {
      beep(80);
      lastBeepTime = millis();
    }
    // Wait for tank_full_cutoff_delay seconds
    if (millis() - tankFullDetectedTime >= (unsigned long)tank_full_cutoff_delay * 1000) {
      turnOffPumpAsync();
      tankFullDelayActive = false;
    }
  } else {
    tankFullDelayActive = false;
  }

}