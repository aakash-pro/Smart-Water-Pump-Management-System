#include "checkpumpoverloadprotection.h"

void checkpumpoverloadprotection()
{
  static unsigned long overloadDetectedTime = 0;
  static bool overloadDelayActive = false;
  static unsigned long overloadLastBeepTime = 0;
  static bool overloadProtectionTripped = false;

  unsigned int overloadThreshold =
      overload_cutoff_power_1 * 1000 +
      overload_cutoff_power_2 * 100 +
      overload_cutoff_power_3 * 10 +
      overload_cutoff_power_4;

  if (overload_cutoff_protection && pump_running && power > overloadThreshold)
  {
    if (!overloadDelayActive)
    {
      overloadDetectedTime = millis();
      overloadDelayActive = true;
    }

    if (overloadDelayActive && !beepCtx.active && (millis() - overloadLastBeepTime >= 100))
    {
      beep(40);
      overloadLastBeepTime = millis();
    }

    if (millis() - overloadDetectedTime >= (unsigned long)overload_cutoff_delay * 1000)
    {
      if (!overloadProtectionTripped)
      {
        turnOffPumpAsync();
        overloadProtectionTripped = true;
      }
      overloadDelayActive = false;
    }


    
  }
  else
  {
    overloadDelayActive = false;
    if (!pump_running) overloadProtectionTripped = false;
  }
}