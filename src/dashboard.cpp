#include "dashboard.h"

inline void drawDashboard_1()
{ 
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);

  // Animation frame (for rotation and water movement)
  static uint16_t pumpAnimFrame = 0;
  static uint16_t waterAnimFrame = 0;
  static uint8_t pumpAnimTick = 0;
  static uint8_t waterAnimTick = 0;
  // Speed up pump impeller (lower value = faster)
  pumpAnimTick = (pumpAnimTick + 1) % 2; // was 4
  if (pumpAnimTick == 0 && pump_running) pumpAnimFrame = (pumpAnimFrame + 1) % 32;
  // Speed up water animation (lower value = faster)
  waterAnimTick = (waterAnimTick + 1) % 2; // was 3
  if (waterAnimTick == 0 && pump_running) waterAnimFrame = (waterAnimFrame + 1) % 64;

  // Tank rectangle and fill
  u8g2.drawFrame(112, 0, 16, 64); // Tank outline
  // Fill tank according to water level (assume level = sum of waterLevel[])
  uint8_t tankLevel = 0;
  for (int i = 0; i < 8; i++) if (waterLevel[i]) tankLevel++;
  if (tankLevel > 0) {
    uint8_t fillHeight = (uint8_t)(tankLevel * 8); // 8px per level
    u8g2.drawBox(113, 64 - fillHeight, 14, fillHeight); // Fill from bottom up
  }

 
  // Pump circle
  u8g2.drawCircle(95, 53, 10, U8G2_DRAW_ALL);
  // Only show rotating lines if pump is running
  if (pump_running) {
    float angleStep = 2 * 3.14159 / 4;
    float animOffset = (pumpAnimFrame / 32.0f) * 2 * 3.14159;
    for (int i = 0; i < 4; i++) {
      float angle = angleStep * i + animOffset;
      int x2 = 95 + (int)(9 * cos(angle));
      int y2 = 53 + (int)(9 * sin(angle));
      u8g2.drawLine(95, 53, x2, y2); // Start from center
    }
  } else {
    // Optionally, show static lines when pump is stopped
    float angleStep = 2 * 3.14159 / 4;
    for (int i = 0; i < 4; i++) {
      float angle = angleStep * i;
      int x2 = 95 + (int)(9 * cos(angle));
      int y2 = 53 + (int)(9 * sin(angle));
      u8g2.drawLine(95, 53, x2, y2); // Start from center
    }
  }

  // Pipes (static)
  u8g2.drawLine(98, 43, 98, 5);
  u8g2.drawLine(98, 5, 111, 5);
  u8g2.drawLine(102, 45, 102, 9);
  u8g2.drawLine(102, 9, 111, 9);
  u8g2.drawLine(60, 63, 60, 50);
  u8g2.drawLine(60, 50, 84, 50);
  u8g2.drawLine(64, 63, 64, 55);
  u8g2.drawLine(64, 55, 85, 55);
 

  // Water flow animation in pipes (larger, spaced particles)
  // Pipe 1: vertical (98,43)-(98,5)
  if(pump_running){
  for (int i = 0; i < 4; i++) {
    int y = 43 - ((waterAnimFrame + i * 10) % 39);
    if (y >= 5 && y <= 43) u8g2.drawBox(99, y, 3, 3);
  }
  
  // Pipe 3: vertical (60,63)-(60,50)
  for (int i = 0; i < 2; i++) {
    int y = 63 - ((waterAnimFrame + i * 7) % 14);
    if (y >= 50 && y <= 63) u8g2.drawBox(61, y, 3, 3);
  }


  // Pipe 5: horizontal (98,5)-(111,5)
  for (int i = 0; i < 3; i++) {
    int x = 98 + ((waterAnimFrame + i * 6) % 14);
    if (x >= 98 && x <= 111) u8g2.drawBox(x, 6, 3, 3);
  }
 
  // Pipe 7: horizontal (60,50)-(84,50)
  for (int i = 0; i < 5; i++) {
    int x = 60 + ((waterAnimFrame + i * 5) % 25);
    if (x >= 60 && x <= 84) u8g2.drawBox(x, 51, 3, 3);
  }

  }
  // Add some live data text
  char buf[20];
  snprintf(buf, sizeof(buf), "Flow: %.1fLPM", currentLPM);
  u8g2.drawStr(0, 10, buf);
  snprintf(buf, sizeof(buf), "Power: %.1fW", power);
  u8g2.drawStr(0, 22, buf);
  snprintf(buf, sizeof(buf), "Volts: %.1fV", voltage);
  u8g2.drawStr(0, 34, buf);
  snprintf(buf, sizeof(buf), "Amps: %.1fA", current);
  u8g2.drawStr(0, 46, buf);
  snprintf(buf, sizeof(buf), "TFP:%s", tank_full_cutoff_protection ? "OFF" : "ON");
  u8g2.drawStr(0, 58, buf);
  

  u8g2.sendBuffer();
}

inline void drawDashboard_2() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);

    // RTC time in 12H format at top
    DateTime now = rtc.now();
    uint8_t hour = now.hour() % 12;
    if (hour == 0) hour = 12;
  char timeBuf[16];
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d:%02d %s", hour, now.minute(), now.second(), now.hour() < 12 ? "AM" : "PM");
    u8g2.drawStr(0, 10, timeBuf);

    // Pump status
    u8g2.drawStr(0, 22, "Pump:");
    u8g2.drawStr(35, 22, pump_running ? "RUNNING" : "STOPPED");

    // Tank status
    u8g2.drawStr(0, 34, "Tank:");
    u8g2.drawStr(35, 34, tank_full ? "FULL" : "NOT FULL");

    // Water level bar
    uint8_t level = 0;
    for (int i = 0; i < 8; i++) if (waterLevel[i]) level++;
    u8g2.drawFrame(0, 46, 64, 8);
    u8g2.drawBox(0, 47, level * 8, 6);
    char lvlBuf[12];
    snprintf(lvlBuf, sizeof(lvlBuf), "Lvl:%d/8", level);
    u8g2.drawStr(70, 54, lvlBuf);

    // Power
    char powerBuf[16];
    snprintf(powerBuf, sizeof(powerBuf), "Pwr:%.1fW", power);
    u8g2.drawStr(0, 62, powerBuf);

    u8g2.sendBuffer();
}

inline void drawDashboard_3() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);

    // RTC time in 12H format at top
    DateTime now = rtc.now();
    uint8_t hour = now.hour() % 12;
    if (hour == 0) hour = 12;
  char timeBuf[16];
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d:%02d %s", hour, now.minute(), now.second(), now.hour() < 12 ? "AM" : "PM");
    u8g2.drawStr(0, 10, timeBuf);

    // Pump icon and status
    u8g2.drawCircle(16, 28, 8, U8G2_DRAW_ALL);
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(6, 41, pump_running ? "Pump ON" : "Pump OFF");

    // Tank icon and status
    u8g2.drawBox(50, 20, 16, 16);
    u8g2.drawStr(52, 41, tank_full ? "Tank FULL" : "Tank OK");

    // Water level as vertical bar
    uint8_t level = 0;
    for (int i = 0; i < 8; i++) if (waterLevel[i]) level++;
    u8g2.drawFrame(80, 20, 8, 16);
    u8g2.drawBox(80, 36 - (level * 2), 8, level * 2);

    // Summary values
    u8g2.setFont(u8g2_font_6x10_tr);
    char buf[20];
    snprintf(buf, sizeof(buf), "Flow:%.1fLPM", currentLPM);
    u8g2.drawStr(0, 56, buf);
    snprintf(buf, sizeof(buf), "V:%.1f I:%.1f", voltage, current);
    u8g2.drawStr(0, 64, buf);

    u8g2.sendBuffer();
}

inline void drawDashboard_4() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);

    // RTC time in 12H format at top
    DateTime now = rtc.now();
    uint8_t hour = now.hour() % 12;
    if (hour == 0) hour = 12;
  char timeBuf[16];
    snprintf(timeBuf, sizeof(timeBuf), "%02d:%02d:%02d %s", hour, now.minute(), now.second(), now.hour() < 12 ? "AM" : "PM");
    u8g2.drawStr(0, 10, timeBuf);

    // Compact summary
    u8g2.drawStr(0, 22, pump_running ? "[P] ON" : "[P] OFF");
    u8g2.drawStr(50, 22, tank_full ? "[T] FULL" : "[T] OK");

    // Water level as horizontal bar
    uint8_t level = 0;
    for (int i = 0; i < 8; i++) if (waterLevel[i]) level++;
    u8g2.drawFrame(0, 32, 80, 8);
    u8g2.drawBox(0, 33, level * 10, 6);

    // Power, Voltage, Current
    char buf[20];
    snprintf(buf, sizeof(buf), "P:%.1fW V:%.1f", power, voltage);
    u8g2.drawStr(0, 46, buf);
    snprintf(buf, sizeof(buf), "I:%.2fA Flow:%.1f", current, currentLPM);
    u8g2.drawStr(0, 56, buf);

    u8g2.sendBuffer();
}

void showDashboard() {
    switch (dashboard_style) {
        case 0:
            drawDashboard_1();
            break;
        case 1:
            drawDashboard_2();
            break;
        case 2:
            drawDashboard_3();
            break;
        case 3:
            drawDashboard_4();
            break;
        
    }
}