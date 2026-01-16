#include "libraries.h"
#include "hardware.h"
#include "data.h"
#include "muimenu.h"
#include "eepromhandler.h"
#include "fetchtank.h"
#include "fetchplug.h"
#include "init.h"
#include "updateRTC.h"
#include "updatestrip.h"
#include "dashboard.h"
#include "pumpcontrol.h"
#include "tankfullprotection.h"



void setup()
{
  strip.begin();
  strip.show();
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.setPixelColor(1, strip.Color(0, 0, 0));
  strip.setPixelColor(2, strip.Color(0, 0, 0));
  strip.setPixelColor(3, strip.Color(0, 0, 0));
  strip.setPixelColor(4, strip.Color(0, 0, 0));
  strip.setPixelColor(5, strip.Color(0, 0, 0));
  strip.setPixelColor(6, strip.Color(0, 0, 0));
  strip.setPixelColor(7, strip.Color(0, 0, 0));
  strip.show();
  pinMode(BTN_SEL, INPUT_PULLUP);
  pinMode(BTN_ESC, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  u8g2.begin(BTN_SEL, U8X8_PIN_NONE, U8X8_PIN_NONE, BTN_UP, BTN_DOWN, BTN_ESC);
  u8g2.setContrast(100);
  u8g2log.begin(u8g2, 32, 8, u8log_buffer);
  u8g2log.setLineHeightOffset(0);
  u8g2log.setRedrawMode(0);
  Wire.begin(3, 1);
  rtc.begin();
  rtc.writeSqwPinMode(PCF8563_SquareWave1Hz);
  rtc.start();
  timeClient.begin();
  u8g2.setFontMode(1);
  mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list) / sizeof(muif_t));
  loadValuesFromEEPROM();
  initialization();

}


void loop()
{
  switch (u8g2.getMenuEvent())
  {
  case U8X8_MSG_GPIO_MENU_SELECT:
    if (mui.isFormActive())
    {
      mui.sendSelect();
      is_redraw = 1;
      beep(40);
    }
    else
    {
      mui.gotoForm(0, 0);
      is_redraw = 1;
      beep(40);
    }
    break;
  case U8X8_MSG_GPIO_MENU_HOME:
    if (mui.isFormActive())
    {
      if (mui.getCurrentFormId() == 0)
      {
        mui.leaveForm();
        is_redraw = 0;
        saveValuesToEEPROM();
        beep(40);
      }
      else
      {
        mui.gotoForm(0, 0);
        is_redraw = 1;
        beep(40);
      }
    }
    
    break;
  case U8X8_MSG_GPIO_MENU_UP:
    if (mui.isFormActive())
    {
      mui.prevField();
      is_redraw = 1;
      beep(40);
    }
    else{
      beep(40);
      turnOnPumpAsync();
    }
    break;
  case U8X8_MSG_GPIO_MENU_DOWN:
    if (mui.isFormActive())
    {
      mui.nextField();
      is_redraw = 1;
      beep(40);
    }
    else{
      beep(40);
      turnOffPumpAsync();
    }
    break;
  }
  if (mui.isFormActive()) {
    if (is_redraw) {
      u8g2.firstPage();
      do {
        mui.draw();
      } while (u8g2.nextPage());
      is_redraw = 0;
    }
  } else {
    static unsigned long lastFetchTank = 0;
    static unsigned long lastFetchPlug = 0;
    unsigned long now = millis();

    if (now - lastFetchTank >= 1000) {
      fetchtank();
      lastFetchTank = now;
    }
    if (now - lastFetchPlug >= 1000) {
      fetchplug();
      lastFetchPlug = now;
    }
   
      showDashboard();
      updateledstrip();
      checktankfullprotection();
      
  }
}










