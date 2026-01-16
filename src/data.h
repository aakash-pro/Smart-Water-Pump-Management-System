#pragma once
#include "libraries.h"

constexpr const char *WIFI_SSID = "HOME-4G";
constexpr const char *WIFI_PASSWORD = "Delhi@1705";
constexpr const char *PUMP_IP = "192.168.50.202";
constexpr const char *TANK_IP = "192.168.50.181";

constexpr uint8_t BTN_DOWN = 2, BTN_UP = 14, BTN_ESC = 12, BTN_SEL = 13;

extern uint8_t dry_run_cutoff_protection, dry_run_cutoff_power_1, dry_run_cutoff_power_2, dry_run_cutoff_power_3, dry_run_cutoff_power_4, dry_run_cutoff_delay;
extern uint8_t tank_full_cutoff_protection, tank_full_cutoff_level, tank_full_cutoff_delay;
extern uint8_t dashboard_style, led_strip_style;
extern volatile uint32_t lastFlowCount;
extern unsigned long lastPoll;
constexpr unsigned long pollInterval = 1000;

//////////Tank Data//////
extern float currentLPM;
extern uint8_t tank_full;
extern uint8_t waterLevel[8];
///////Plug Data///////
extern uint8_t pump_running;
extern float power;
extern float voltage;
extern float current;
extern float ApparentPower;
extern float ReactivePower;
extern float PowerFactor;
extern float TotalEnergy;
extern float yesterdayEnergy;
extern float todayEnergy;

///////////////////////
extern bool myFlag;
extern uint8_t is_redraw;
extern uint8_t u8log_buffer[];

