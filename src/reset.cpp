#include "reset.h"


void reset() {
  clearEEPROM();
  wm.resetSettings();
  WiFi.disconnect(true);
  delay(200);
 

}