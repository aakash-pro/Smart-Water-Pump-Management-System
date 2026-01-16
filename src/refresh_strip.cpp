#include "refresh_strip.h"


void refresh_led_strip()
{
  // Helper to fill LEDs progressively with the given color
  auto fillColorProgressive = [&](uint8_t r, uint8_t g, uint8_t b)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        if (j <= i)
        {
          strip.setPixelColor(j, strip.Color(r, g, b));
        }
        else
        {
          strip.setPixelColor(j, strip.Color(0, 0, 0));
        }
      }
      strip.show();
      delay(50);
    }
  };

  // Helper to turn all LEDs off
  auto clearStrip = [&]()
  {
    for (int i = 0; i < 8; i++)
    {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
    delay(100);
  };

  fillColorProgressive(0, 0, 5); // blue
  clearStrip();

  fillColorProgressive(0, 5, 0); // green
  clearStrip();

  fillColorProgressive(5, 0, 0); // red
  clearStrip();
}
  

