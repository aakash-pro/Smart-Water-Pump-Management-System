#include "hardware.h"

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, D1, D2, U8X8_PIN_NONE, D0, U8X8_PIN_NONE);
U8G2LOG u8g2log;
MUIU8G2 mui;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 19800, 60000);
Adafruit_NeoPixel strip(8, D3, NEO_GRB + NEO_KHZ800);
RTC_PCF8563 rtc;
WiFiManager wm;