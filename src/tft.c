#include "tft.h"
#include "myfun.h"

extern window TFT_Win;
									
void TFT_Write_Data(unsigned char dat)
{
 TFT_DC=1; //选为数据模式
 TFT_CS=0; //打开片选 

 TFT_PORT=dat; //将数据(1个字节)写到端口上
 TFT_WR=0; //打开写使能	 在此期间数据被写入
 TFT_WR=1; //关闭片选 

 TFT_CS=1; //关闭片选 
}

void TFT_Write_Data16(unsigned char dataH,unsigned char dataL)
{
 TFT_DC=1;
 TFT_CS=0;
 TFT_PORT=dataH;
 TFT_WR=0;
 TFT_WR=1;
 TFT_PORT=dataL;
 TFT_WR=0;
 TFT_WR=1;
 TFT_CS=1;
}

void TFT_Write_Cmd(unsigned char cmd)
{
 TFT_DC=0;
 TFT_CS=0;

 TFT_PORT=0;
 TFT_WR=0;
 TFT_WR=1;

 TFT_PORT=cmd;
 TFT_WR=0;
 TFT_WR=1;

 TFT_CS=1;	
}

void TFT_Write_Reg(unsigned char reg,unsigned int data16)
{
 TFT_Write_Cmd(reg); 	
 TFT_Write_Data16(data16/256,data16);
}

void TFT_Set_Window(unsigned char x_start,unsigned char x_end,unsigned char y_start,unsigned char y_end)
{
 TFT_Win.x_start=x_start;
 TFT_Win.x_end  =x_end;
 TFT_Win.y_start=y_start;
 TFT_Win.y_end  =y_end;
 TFT_Write_Reg(0x44,((unsigned int)(TFT_Win.x_end+4))*256+(TFT_Win.x_start+4));
 TFT_Write_Reg(0x45,((unsigned int)TFT_Win.y_end)*256+(TFT_Win.y_start));
}

void TFT_MoveTo(unsigned char x,unsigned char y)
{
 TFT_Write_Reg(0x21,(((unsigned int)(TFT_Win.y_start+y))*256)+(TFT_Win.x_start+x));
}

void TFT_Clr(unsigned int color)
{
 unsigned int i=0,length=0;
 length=((unsigned int)(TFT_Win.x_end-TFT_Win.x_start+1))*((unsigned int)(TFT_Win.y_end-TFT_Win.y_start+1));
 TFT_MoveTo(0,0);
 TFT_Write_Cmd(0x22);
 for(i=0;i<length;i++)
  TFT_Write_Data16(color/256,color);  
}

void TFT_Draw_Point(unsigned char x,unsigned char y,unsigned int color)
{
 TFT_MoveTo(x,y);
 TFT_Write_Cmd(0x22);
 TFT_Write_Data16(color/256,color);
}

void TFT_Reset()
{
 TFT_RST=1;
 delay(10);
 TFT_RST=0;
 delay(10);
 TFT_RST=1;
 delay(10);
}

void TFT_Init()
{
 TFT_Write_Reg(0x00,0x0001);
 delay(2500);
 TFT_Write_Reg(0x01,0x010f);
 TFT_Write_Reg(0x02,0x0700);
 TFT_Write_Reg(0x03,0x1030);

 TFT_Write_Reg(0x10,0x2004);
 TFT_Write_Reg(0x12,0x0109);  
 //TFT_Write_Reg(0x13,0x0e1d);
 TFT_Write_Reg(0x13,0x0e5d);
 TFT_Write_Reg(0x14,0x6224);

 TFT_Write_Reg(0x07,0x0000);
 TFT_Write_Reg(0x08,0x0202);
 TFT_Write_Reg(0x0b,0x0001);
 TFT_Write_Reg(0x0c,0x0000);
 TFT_Write_Reg(0x40,0x0000);
 TFT_Write_Reg(0x42,0x8300);	
 TFT_Write_Reg(0x43,0x0000);
 TFT_Write_Reg(0x44,0x8304);
 TFT_Write_Reg(0x45,0x7f00);
 TFT_Write_Reg(0x25,0x0002);
 TFT_Write_Reg(0x26,0x0002);
 TFT_Write_Reg(0x30,0x0000);
 TFT_Write_Reg(0x31,0x0102);
 TFT_Write_Reg(0x32,0x0001);
 TFT_Write_Reg(0x33,0x0202);	
 TFT_Write_Reg(0x34,0x0707);
 TFT_Write_Reg(0x35,0x0707);
 TFT_Write_Reg(0x36,0x0707);
 TFT_Write_Reg(0x37,0x0202);
 TFT_Write_Reg(0x07,0x0005);		
 TFT_Write_Reg(0x07,0x0015);
 TFT_Write_Reg(0x21,0x0004);	
 TFT_Write_Reg(0x21,0x0004);
 TFT_Write_Reg(0x44,0x8304);
 TFT_Write_Reg(0x45,0x7f00);

 TFT_Write_Cmd(0x22);
 	
 TFT_Write_Reg(0x10,0x0000);
 TFT_Write_Reg(0x03,0x1030);	
 TFT_Write_Reg(0x10,0x2004);
 TFT_Write_Reg(0x13,0x0e5d);	
 TFT_Write_Reg(0x07,0x0017);	
 TFT_Write_Cmd(0x22);	
}
