#include "checkpumpdryrunprotection.h"

void checkpumpdryrunprotection()
{
    static unsigned long dryRunDetectedTime = 0;
    static bool dryRunDelayActive = false;
    static unsigned long dryRunLastBeepTime = 0;
    static bool dryRunProtectionTripped = false; // latch to prevent multiple shutdowns during same event

    // Calculate dry run threshold
    unsigned int dryRunThreshold =
        dry_run_cutoff_power_1 * 1000 +
        dry_run_cutoff_power_2 * 100 +
        dry_run_cutoff_power_3 * 10 +
        dry_run_cutoff_power_4;

    if (dry_run_cutoff_protection && pump_running && power < dryRunThreshold)
    {
        if (!dryRunDelayActive)
        {
            dryRunDetectedTime = millis();
            dryRunDelayActive = true;
        }

        // Beep every 100ms during delay
        if (dryRunDelayActive && !beepCtx.active && (millis() - dryRunLastBeepTime >= 100))
        {
            beep(40);
            dryRunLastBeepTime = millis();
        }

        if (millis() - dryRunDetectedTime >= (unsigned long)dry_run_cutoff_delay * 1000)
        {
            if (!dryRunProtectionTripped)
            {
                turnOffPumpAsync();
                dryRunProtectionTripped = true;
            }
            dryRunDelayActive = false;
        }

        
    }
    else
    {
        dryRunDelayActive = false;
        if (!pump_running) dryRunProtectionTripped = false;
    }
}