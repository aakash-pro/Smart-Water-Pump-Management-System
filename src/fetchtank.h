#pragma once
#include "libraries.h"
#include "data.h"

struct TankContext {
    AsyncClient client;
    bool requestInProgress = false;
    bool initialized = false;
    char buffer[768] = {0};
    bool readyForNext = true;
    unsigned long retryAfter = 0;
    size_t len = 0;
};

extern TankContext tankCtx;
void initTankClient();
void fetchtank();
