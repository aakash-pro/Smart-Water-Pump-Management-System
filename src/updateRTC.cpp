#include "updateRTC.h"


bool updateRTCFromNTP()
{

  unsigned long epochTime = timeClient.getEpochTime();
  DateTime ntpTime(epochTime);

  rtc.adjust(ntpTime);

  delay(100);

  DateTime rtcTime = rtc.now();

  int64_t diff = (int64_t)rtcTime.unixtime() - (int64_t)ntpTime.unixtime();
  if (diff < 0)
  {
    diff = -diff;
  }

  if (diff <= 5)
  {
    return true;
  }
  else
  {
    return false;
  }
}
