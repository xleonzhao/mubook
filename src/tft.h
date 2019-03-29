#ifndef __TFT_H__
#define __TFT_H__

#include "reg51.h"

#define START_X 4
#define START_Y 0
#define MAX_X   127
#define MAX_Y   127

//TFT液晶端口定义
#define TFT_PORT  P0 //数据命令端口
sbit TFT_CS  = P2^6; //片选 
sbit TFT_RST = P2^2; //复位
sbit TFT_DC  = P2^5; //命令与数据切换
sbit TFT_WR  = P3^6; //写使能
sbit TFT_RD  = P3^7; //读使能

#define COLOR_WHITE 0xffff
#define COLOR_BLACK 0x0000

typedef struct Win
{
 unsigned char x_start;
 unsigned char x_end;
 unsigned char y_start;
 unsigned char y_end;
} window;

//函数声明
void TFT_Write_Data(unsigned char dat);
void TFT_Write_Data16(unsigned char dataH,unsigned char dataL);
void TFT_Write_Cmd(unsigned char dat);
void TFT_Write_Reg(unsigned char reg,unsigned int data16);
void TFT_Init();
void TFT_Reset();
void TFT_Set_Window(unsigned char x_start,unsigned char x_end,unsigned char y_start,unsigned char y_end);
void TFT_MoveTo(unsigned char x,unsigned char y);
void TFT_Draw_Point(unsigned char x,unsigned char y,unsigned int color);
void TFT_Flood_Data(unsigned int *pcolor,unsigned int length);
void TFT_Clr(unsigned int color);

#endif