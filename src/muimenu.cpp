#include "muimenu.h"


uint8_t mui_hrule(mui_t *ui, uint8_t msg)
{
  if (msg == MUIF_MSG_DRAW)
    u8g2_DrawHLine(mui_get_U8g2(ui), 0, mui_get_y(ui), u8g2_GetDisplayWidth(mui_get_U8g2(ui)));
  return 0;
}

