
#pragma once
#include "libraries.h"
#include "hardware.h"
#include "data.h"
#include "updateRTC.h"
#include "refresh_strip.h"
#include "beep.h"

void showLoadingBar(uint8_t percent, const char *status);
const char *wifiStatusToString(wl_status_t status);
bool checkStatus(const char *ip, uint32_t timeout_ms = 4000);
bool connectWifi(uint32_t timeout_ms = 15000);
bool syncTime(uint32_t timeout_ms = 8000);
void initialization();
void comm_error(int wifiFail, int plugFail, int tankFail);