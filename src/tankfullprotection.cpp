#include "tankfullprotection.h"

void checktankfullprotection()
{

  static unsigned long tankFullDetectedTime = 0;
  static bool tankFullDelayActive = false;
  static unsigned long lastBeepTime = 0;
  static bool tankFullProtectionTripped = false;
  if (!tank_full_cutoff_protection && pump_running && waterLevel[tank_full_cutoff_level - 1])
  {
    if (!tankFullDelayActive)
    {
      tankFullDetectedTime = millis();
      tankFullDelayActive = true;
    }

    if (tankFullDelayActive && !beepCtx.active && (millis() - lastBeepTime >= 100))
    {
      beep(80);
      lastBeepTime = millis();
    }

    if (millis() - tankFullDetectedTime >= (unsigned long)tank_full_cutoff_delay * 1000)
    {
      if (!tankFullProtectionTripped)
      {
        turnOffPumpAsync();
        tankFullProtectionTripped = true;
      }
      tankFullDelayActive = false;
    }

    
  }
  else
  {
    tankFullDelayActive = false;
    if (!pump_running) tankFullProtectionTripped = false;
  }
}