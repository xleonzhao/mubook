#include "myfun.h"

void delay(UINT16 time) 
{
 while(time--);
}

void delay_ms(UINT16 xms) 
{
  /* this MCU runs a clcok of 22MHz and each clock runs an instruction.
     to get a 1ms delay, you need run 22K instructions.
  */
  UINT16 i,j;
  while (xms--)
    for (i = 0; i<22; i++)
      for (j = 0; j<1000; j++);
}

