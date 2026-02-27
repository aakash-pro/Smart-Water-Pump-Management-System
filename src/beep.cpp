#include "beep.h"

BeepContext beepCtx;

void beep(uint8_t duration_ms) {
    if (beepCtx.active) return;  // ignore if already beeping
    digitalWrite(BUZZER_PIN, HIGH);
    beepCtx.active = true;
    beepCtx.startTime = millis();
    beepCtx.duration = duration_ms;
}

void handleBeep() {
    if (!beepCtx.active) return;
    if (millis() - beepCtx.startTime >= beepCtx.duration) {
        digitalWrite(BUZZER_PIN, LOW);
        beepCtx.active = false;
    }
}