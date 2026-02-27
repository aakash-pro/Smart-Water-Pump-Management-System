#pragma once
#include "libraries.h"
#include "data.h"

struct PumpContext {
    bool requestInProgress = false;
    bool responseReceived = false;
    char command[8] = {0};
    char lastKnownState[8] = {0};
    char buffer[256] = {0};
    size_t len = 0;
    unsigned long retryAfter = 0;
};

extern PumpContext pumpCtx;  // exposed so main.cpp can read lastKnownState

void initPumpClient();
void pumpRequest(const char* command);
void turnOnPumpAsync();
void turnOffPumpAsync();

