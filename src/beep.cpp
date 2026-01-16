#include "beep.h"

void beep(uint8_t duration_ms)
{
  digitalWrite(BUZZER_PIN, HIGH);
  delay(duration_ms);
  digitalWrite(BUZZER_PIN, LOW);
}