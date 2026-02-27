#pragma once
#include "libraries.h"
#include "data.h"

#define PLUG_PORT 80

// ─── Relay Status Context ────────────────────────────────────────────
struct RelayContext {
    AsyncClient client;
    bool requestInProgress = false;
    bool initialized = false;
    char buffer[256] = {0};
    bool readyForNext = true;
    unsigned long retryAfter = 0;
    size_t len = 0;
};

// ─── Energy Context ──────────────────────────────────────────────────
struct EnergyContext {
    AsyncClient client;
    bool requestInProgress = false;
    bool initialized = false;
    char buffer[768] = {0};  // larger — Status 10 response is bigger
    bool readyForNext = true;
    unsigned long retryAfter = 0;
    size_t len = 0;
};

extern RelayContext relayCtx;
extern EnergyContext energyCtx;

void initRelayClient();
void initEnergyClient();
void fetchplug();
void fetchplugEnergy();

