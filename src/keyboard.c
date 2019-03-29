#include<reg51.h>
#include "uart.h"
#include "mytype.h"
#include "myfun.h"
#include "keyboard.h"

sbit K1=P1^0;
sbit K2=P1^1;
sbit K3=P1^2;
sbit K4=P1^3;
sbit K7=P1^7;

UINT8 wait_for_key(UINT8 auto_read)
{
  UINT8 key;
  UINT8 i=0;

	P1=0xff; // reset key status
	while(1)
	{
		if(K1==0)
		{
			delay_ms(10);		//delay 10ms to reduce nosie
			if(K1==0)
			{
#ifdef debug
				UART_Send_Str("UP key pressed.\n");
#endif		
        key = PAGE_UP;			
				break;
			}
		}
		if(K2==0)
		{
			delay_ms(10);		//delay 10ms to reduce nosie
			if(K2==0)
			{
#ifdef debug
				UART_Send_Str("DOWN key pressed.\n");
#endif					
        key = PAGE_DOWN;
				break;
			}
		}
		if(K3==0)
		{
			delay_ms(10);		//delay 10ms to reduce nosie
			if(K3==0)
			{
#ifdef debug
				UART_Send_Str("ENTER key pressed.\n");
#endif					
        key = KEY_ENTER;
				break;
			}
		}
		if(K4==0)
		{
			delay_ms(10);		//delay 10ms to reduce nosie
			if(K4==0)
			{
#ifdef debug
				UART_Send_Str("ESC key pressed.\n");
#endif					
        key = KEY_ESC;
				break;
			}
		}
    if(auto_read) {
      i++;
      if(i>100) {
        key = PAGE_DOWN;
        break;
      }
      delay_ms(10);
      K7 = 0;
    } else {
      K7 = 1;
    }
	}
	P1=0xff; //reset key
  return key;
}