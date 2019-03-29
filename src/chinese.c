#include "tft.h"
#include "chinese.h"
#include "znFAT.h"
#include "uart.h"
#include "mytype.h"
#include "debug.h"
#include "string.h"

data struct FileInfoStruct HZK16FileInfo;
data UINT8 cchar_dots[CCHAR_BYTES];
code UINT8 default_cchar_dots[CCHAR_BYTES] = {
0x00,0x00,0x7F,0xFE,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,
0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x40,0x02,0x7F,0xFE,0x00,0x00,
};

void init_chinese()
{
  /* open HZK16 file */
  if(znFAT_Open_File(&HZK16FileInfo, HZK16_FILE, 0, 1))
  {  
#ifdef debug
    UART_Send_Str("打开HZK16文件失败\n");
#endif
    return;
  }

#ifdef debug
    UART_Send_Str("打开HZK16文件成功\n"); //从串口输出文件参数信息
    UART_Send_Str("文件名为：");
    UART_Send_Str(HZK16FileInfo.FileName);
    UART_Send_Enter();
    UART_Put_Inf("文件大小(字节)：",HZK16FileInfo.FileSize);
    UART_Put_Inf("文件当前偏移量(字节)：",HZK16FileInfo.FileCurOffset);
    UART_Send_Enter();
#endif
}

/* given a chinese character code (by GB2312 standard, return                   */
/* a dots matrix of how to draw that character by dots.		                    */
/* if success, return 1 indicating use chinese_char_dots[16] where dots are stored  */
/* if fail like not found in HZK16, return 0 to default_cchar_dots [16] */
UINT8 get_chinese_char_dots(UINT8 dataH, UINT8 dataL)
{
  UINT8 len=0, i;
  UINT32 offset = 0;

  offset = dataL-0xa0-1;
  offset = offset +  (dataH-0xa0-1) * 94;
  offset = offset * CCHAR_BYTES;

  /* FIXME: why we need this? because there is a bug in znFAT.c, if you read
     same char twice, it will shift by 16 bytes. try reading two 学 */
  znFAT_Seek_File(&HZK16FileInfo, 0);
   							 
  if(len=znFAT_Read_File(&HZK16FileInfo, offset, CCHAR_BYTES, cchar_dots))
  {
#ifdef debug_chinese
    UART_Send_Str("读取文件成功\n");	//串口输出读到的数据相关信息
    UART_Put_Inf("共读出字节：",len);
    UART_Send_Str("读到的内容是：\n");
    UART_Send_Byte(dataH);
    UART_Send_Byte(dataL);
	  UART_Send_Enter();
    UART_Put_Inf("文件读取偏移量(字节)：",offset);
    UART_Put_Inf("文件当前偏移量(字节)：",HZK16FileInfo.FileCurOffset);
	  UART_Send_Enter();

        for(i=0;i<len;i++)	//将读到的数据通过串口发出来
        {
          UART_Put_Inf(" ", cchar_dots[i]);
        }     

#endif
  }
  if (len == 32) {
    return 1;
  }	else {
#ifdef debug_chinese
      UART_Send_Str("didn't find anything in HZK16.\n");
      UART_Put_Inf("共读出字节：",len);
#endif
    return 0;
  }
}

void display_chinese_char(UINT8 cx, UINT8 cy, UINT8 charH, UINT8 charL)
{
  UINT8 j=0, k=0;
  UINT16 dx;
  UINT16 * pdots;
  UINT8 px = cx;

  if(get_chinese_char_dots(charH, charL)) {
    pdots = (UINT16 *) cchar_dots;
  } else {
    pdots = (UINT16 *) default_cchar_dots;
  }

  /* draw a single cchar */
  for (j=0; j<CCHAR_HEIGHT; j++)
  {
    dx = pdots[j];
    /* draw a single line of cchar */
    for (k=0; k<CCHAR_WIDTH; k++) 
    { 
      if (dx & 0x8000) {
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
