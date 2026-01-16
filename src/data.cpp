#include "data.h"

uint8_t dry_run_cutoff_protection = 1;
uint8_t dry_run_cutoff_power_1 = 1;
uint8_t dry_run_cutoff_power_2 = 0;
uint8_t dry_run_cutoff_power_3 = 0;
uint8_t dry_run_cutoff_power_4 = 0;
uint8_t dry_run_cutoff_delay = 0;
uint8_t tank_full_cutoff_protection = 1;
uint8_t tank_full_cutoff_level = 8;
uint8_t tank_full_cutoff_delay = 10;
uint8_t dashboard_style = 1;
uint8_t led_strip_style = 1;


volatile uint32_t lastFlowCount = 0;
unsigned long lastPoll = 0;

////////Tank Data//////
float currentLPM = 0.0;
uint8_t tank_full = 0;
uint8_t waterLevel[8] = {0};
/////////Plug data////////
uint8_t pump_running = 1;
float power = 0.0;
float voltage = 0.0;
float current = 0.0;
float ApparentPower = 0.0;
float ReactivePower = 0.0;
float PowerFactor = 0.0;
float TotalEnergy = 0.0;
float yesterdayEnergy = 0.0;
float todayEnergy = 0.0;
////////////////////////////
bool myFlag = false;
uint8_t is_redraw = 0;
uint8_t u8log_buffer[32 * 8];
