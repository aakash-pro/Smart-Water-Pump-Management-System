#include "init.h"


void showLoadingBar(uint8_t percent, const char *status)
{
  static uint8_t frame = 0;
  for (; frame <= percent && frame <= 100; ++frame)
  {
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.drawStr(0, 7, "Automatic Water Pump");
      u8g2.drawStr(0, 15, "Controller");
      if (WiFi.SSID().length() == 0)
      {
        u8g2.drawStr(0, 26, "Setup WiFi Connection");
        u8g2.drawStr(0, 35, "Connect :192.168.4.1");
      }
      else
      {
        u8g2.drawStr(0, 26, "SSID:");
        u8g2.drawStr(36, 26, WiFi.SSID().c_str());
      }
      if (WiFi.SSID().length() == 0)
      {
       u8g2.drawStr(0, 50, "Status: No Network");
      }
      else
      {
      u8g2.drawStr(0, 50, "Status:");
      u8g2.drawStr(42, 50, status);
    }
      int boxwidth = (frame * 107) / 100;
      u8g2.drawFrame(0, 58, 108, 6);
      u8g2.drawBox(0, 59, boxwidth, 4);

      char buf[6];
      snprintf(buf, sizeof(buf), "%3d", frame);
      u8g2.drawStr(109, 64, buf);
    } while (u8g2.nextPage());
    yield();
  }
  frame = percent;
}

const char *wifiStatusToString(wl_status_t status)
{
  switch (status)
  {
  case WL_IDLE_STATUS:
    return "Idle";
  case WL_NO_SSID_AVAIL:
    return "No SSID";
  case WL_SCAN_COMPLETED:
    return "Scan Done";
  case WL_CONNECTED:
    return "Connected";
  case WL_CONNECT_FAILED:
    return "Failed";
  case WL_CONNECTION_LOST:
    return "Lost";
  case WL_DISCONNECTED:
    return "Connecting";
  default:
    return "Unknown";
  }
}

bool connectWifi(uint32_t timeout_ms)
{
  
  
  res = wm.autoConnect("Pump Controller", "12345678");
  if (!res)
    return false;

  uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - start > timeout_ms)
      return false;
    showLoadingBar(10, wifiStatusToString(WiFi.status()));
    delay(100);
    yield();
  }
  return true;
}

bool checkStatus(const char *ip, uint32_t timeout_ms)
{
  HTTPClient http;
  WiFiClient client;
  String url = String("http://") + ip + "/cm?cmnd=Status%2010";
  uint32_t start = millis();
  while (millis() - start < timeout_ms)
  {
    http.begin(client, url);
    int code = http.GET();
    if (code == HTTP_CODE_OK)
    {
      String payload = http.getString();
      if (payload.indexOf("StatusSNS") >= 0)
      {
        http.end();
        return true;
      }
    }
    http.end();
    delay(300);
    yield();
  }
  return false;
}

bool syncTime(uint32_t timeout_ms)
{
  uint32_t start = millis();
  while (millis() - start < timeout_ms)
  {
    if (timeClient.update())
      return true;
    delay(300);
    yield();
  }
  return false;
}

void comm_error(int wifiFail, int plugFail, int tankFail)
{
  myFlag = !myFlag;
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawStr(0, 9, "COMM FAILURE DETECTED");
  u8g2.drawHLine(0, 12, 128);
  u8g2.setFont(u8g2_font_6x10_tr);
  if (wifiFail)
  {
    if (myFlag)
    {
      u8g2.drawStr(0, 22, "[ ] WiFi: Error");
    }
    else
    {
      u8g2.drawStr(0, 22, "[x] WiFi: Error");
    }
  }
  else
  {
    u8g2.drawStr(0, 22, "[ ] WiFi: Connected");
  }
  if (plugFail)
  {
    if (myFlag)
    {
      u8g2.drawStr(0, 34, "[ ] Water Pump: Error");
    }
    else
    {
      u8g2.drawStr(0, 34, "[x] Water Pump: Error");
    }
  }
  else
  {
    u8g2.drawStr(0, 34, "[ ] Water Pump: OK");
  }
  if (tankFail)
  {
    if (myFlag)
    {
      u8g2.drawStr(0, 48, "[ ] Tank: Error");
    }
    else
    {
      u8g2.drawStr(0, 48, "[x] Tank: Error");
    }
  }
  else
  {
    u8g2.drawStr(0, 48, "[ ] Tank: OK");
  }
  u8g2.drawHLine(0, 51, 128);
  if (myFlag)
  {
    u8g2.drawStr(0, 62, "Fallback: ");
  }
  else
  {
    u8g2.drawStr(0, 62, "Fallback:Troubleshoot");
  }
  u8g2.sendBuffer();
}

void initialization()
{

    showLoadingBar(0, "Booting");
  if (!connectWifi())
  {
    showLoadingBar(15, "WiFi Failed");
    delay(100);
    while (true)
    {
      delay(100);
      comm_error(1, 0, 0);
    }
  }
  showLoadingBar(20, "WiFi Connected");
  delay(100);
  showLoadingBar(25, "Checking Pump");
  if (!checkStatus(PUMP_IP))
  {
    showLoadingBar(30, "Pump Error");
    delay(100);
    while (true)
    {
      delay(100);
      comm_error(0, 1, 0);
    }
  }
  delay(100);
  showLoadingBar(35, "Pump Connected");
  delay(100);
  showLoadingBar(40, "Checking Tank");
  if (!checkStatus(TANK_IP))
  {
    showLoadingBar(45, "Tank Error");
    delay(100);
    while (true)
    {
      delay(100);
      comm_error(0, 0, 1);
    }
  }
  delay(100);
  showLoadingBar(50, "Tank OK");
  delay(100);
  showLoadingBar(55, "Syncing NTP");
  delay(100);
  if (!syncTime())
  {
    showLoadingBar(60, "NTP Sync Fail");
    while (!syncTime())
    {
      delay(100);
      showLoadingBar(60, "NTP Retry..");
      yield();
    }
  }
  delay(100);
  showLoadingBar(65, "NTP OK");
  delay(100);
  showLoadingBar(65, "Updating RTC");
  delay(100);
  if (!updateRTCFromNTP())
  {
    showLoadingBar(70, "RTC: failed");
    while (!updateRTCFromNTP())
    {
      delay(100);
      showLoadingBar(70, "RTC: Retry..");
      yield();
    }
  }
  delay(100);
  showLoadingBar(70, "RTC Updated");
  delay(100);
  showLoadingBar(80, "LED initializing");
  refresh_led_strip();
  delay(100);
  showLoadingBar(90, "LED initialized");
  delay(100);
  showLoadingBar(100, "All healthy");
  delay(100);
  beep(40);
}