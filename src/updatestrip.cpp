#include "updatestrip.h"

 
 void updateledstrip() {
 strip.clear();
    for (int i = 0; i < 8; i++) {
      // Reverse mapping: LED[7-i] shows waterLevel[i]
      if (waterLevel[i] == 1) {
        strip.setPixelColor(7 - i, strip.Color(0, 4, 0));
      } else {
        strip.setPixelColor(7 - i, strip.Color(4, 0, 0));
      }
    }
    strip.show();
}