#pragma once
#include "libraries.h"

constexpr uint8_t BUZZER_PIN = 15;

struct BeepContext {
    bool active = false;
    uint32_t startTime = 0;
    uint32_t duration = 0;
};

extern BeepContext beepCtx;  // exposed so protection functions can check beepCtx.active

void beep(uint8_t duration_ms);
void handleBeep();