#include "tft.h"
#include "english.h"
#include "uart.h"
#include "mytype.h"
#include "debug.h"
#include "string.h"

void display_english_char(UINT8 cx, UINT8 cy, UINT8 echar)
{
  UINT8 j=0, k=0;
  UINT8 dx;
  UINT8 * pdots;
  UINT8 px = cx;

  pdots = ASCII816[echar-32];

#ifdef debug_english
  UART_Send_Str("printing english chars: ");	
  UART_Send_Byte(echar);
  UART_Send_Enter();	
#endif

  /* draw a single english char */
  for (j=0; j<ECHAR_HEIGHT; j++)
  {
    dx = pdots[j];
    /* draw a single line of cchar */
    for (k=0; k<ECHAR_WIDTH; k++) 
    { 
      if (dx & 0x80) {
        TFT_Draw_Point(cx, cy, COLOR_WHITE);
      } else {
        TFT_Draw_Point(cx, cy, COLOR_BLACK);
      }
      dx = dx << 1;
      cx++;
    }
    /* line return and move downward one line */
    cx=px;
    cy++;
  }
}
