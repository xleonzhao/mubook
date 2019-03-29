#include "uart.h"
#include "znFAT.h"
#include "book.h"
#include "string.h"
#include "debug.h"
#include "reg51.h"
#include "tft.h"
#include "math.h"
#include "chinese.h"

/**************************************************************

         +------------------------------------------+
         |  振南电子 高级外设测试 之  读取文件数据  |
         +------------------------------------------+

  此源码版权属 振南 全权享有，如欲引用，敬请署名并告知
        严禁随意用于商业目的，违者必究，后果自负
         振南电子 
             ->产品网站 http://www.znmcu.cn/
             ->产品网店 http://shop.znmcu.cn/
             ->产品咨询 QQ :987582714 
			            MSN:yzn07@126.com
	                    WW :yzn07
  程序说明：本程序调用znFAT的读取文件数据函数，对\test.txt的数据
            进行读取，并将读到的数据发到串口。
  注：SD卡相关教程请参见 
     《振南的疯狂单片机(51)》高级外围篇 之 《SD卡【II】》				  
**************************************************************/

unsigned char Dev_No; //设备号
struct znFAT_Init_Arg *pArg; //用于指针文件系统参数集合的指针
struct znFAT_Init_Arg Init_Arg_SDCARD; //文件系统参数集合，用于记录文件系统的重要参数

BOOK_INFO lastbook;

window TFT_Win; /* this variable has to be defined here. why? */

void init_all()
{
  P1=0x00; //lit all lights

#ifdef debug
  /* init serial port for debug */
  UART_Init(); //串口初始化
#endif

  /* init SD device */
  znFAT_Device_Init(); //存储设备初始化
  pArg=&Init_Arg_SDCARD;	//指针指向SD卡文件系统参数集合，znFAT将从这个集合中获取参数
  Dev_No=SDCARD;	//设备号为SDCARD，znFAT依照此设备号选择存储设备驱动

  /* init znFAT file system */ 
  znFAT_Init();	//文件系统初始化
#ifdef debug
    UART_Put_Inf("总存储容量：(字节)",pArg->Total_Size);//从串口输出各种参数信息
    UART_Put_Inf("每扇区字节：(个)",pArg->BytesPerSector);
    UART_Put_Inf("每簇扇区：(个)",pArg->SectorsPerClust);
#endif

  /* init TFT */
  TFT_Reset(); 	  
  TFT_Init();
  TFT_Set_Window(0,127,0,127);
  TFT_Clr(COLOR_BLACK);

  /* init chinese HZK16 file */
  init_chinese();

  P1=0xff; //init finished.
}

void main()
{
  init_all();

  read_last_book(&lastbook);

  while(1)
  {
    read_book(&lastbook);
  }
}