#include "eepromhandler.h"


void loadValuesFromEEPROM()
{
  EEPROM.begin(512);
  uint8_t *vars[] = {
      &dry_run_cutoff_protection, &dry_run_cutoff_power_1, &dry_run_cutoff_power_2, &dry_run_cutoff_power_3, &dry_run_cutoff_power_4,
      &dry_run_cutoff_delay, &tank_full_cutoff_protection, &tank_full_cutoff_level, &tank_full_cutoff_delay, &dashboard_style, &led_strip_style};
  constexpr uint8_t addrs[] = {
      EEPROM_dry_run_cutoff_protection_ADDR, EEPROM_dry_run_cutoff_power_1_ADDR, EEPROM_dry_run_cutoff_power_2_ADDR,
      EEPROM_dry_run_cutoff_power_3_ADDR, EEPROM_dry_run_cutoff_power_4_ADDR, EEPROM_dry_run_cutoff_delay_ADDR,
      EEPROM_tank_full_cutoff_protection_ADDR, EEPROM_tank_full_cutoff_level_ADDR, EEPROM_tank_full_cutoff_delay_ADDR,
      EEPROM_dashboard_style_ADDR, EEPROM_led_strip_style_ADDR};
  for (size_t i = 0; i < sizeof(vars) / sizeof(vars[0]); ++i)
  {
    uint8_t val = EEPROM.read(addrs[i]);
    if (val != 0xFF)
      *vars[i] = val;
  }
  EEPROM.end();
}

void saveValuesToEEPROM(){
  EEPROM.begin(512);
  const uint8_t vals[] = {
      dry_run_cutoff_protection, dry_run_cutoff_power_1, dry_run_cutoff_power_2, dry_run_cutoff_power_3, dry_run_cutoff_power_4,
      dry_run_cutoff_delay, tank_full_cutoff_protection, tank_full_cutoff_level, tank_full_cutoff_delay, dashboard_style, led_strip_style};
  constexpr uint8_t addrs[] = {
      EEPROM_dry_run_cutoff_protection_ADDR, EEPROM_dry_run_cutoff_power_1_ADDR, EEPROM_dry_run_cutoff_power_2_ADDR,
      EEPROM_dry_run_cutoff_power_3_ADDR, EEPROM_dry_run_cutoff_power_4_ADDR, EEPROM_dry_run_cutoff_delay_ADDR,
      EEPROM_tank_full_cutoff_protection_ADDR, EEPROM_tank_full_cutoff_level_ADDR, EEPROM_tank_full_cutoff_delay_ADDR,
      EEPROM_dashboard_style_ADDR, EEPROM_led_strip_style_ADDR};
  for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); ++i)
    EEPROM.write(addrs[i], vals[i]);
  EEPROM.commit();
  EEPROM.end();
}

void clearEEPROM() {
  EEPROM.begin(512);
  for (int i = 0; i < 512; ++i) {
    EEPROM.write(i, 0xFF); // 0xFF is the default erased value
  }
  EEPROM.commit();
  EEPROM.end();
}