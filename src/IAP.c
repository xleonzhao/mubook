#include "IAP.h"
#include "mytype.h"
#include <string.h>

void IAP_disable(void)
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}

UINT8 IAP_Read_Byte(UINT16 addr)
{
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM 字节读命令

    IAP_ADDRH = (addr & 0xFF00)>>8;
    IAP_ADDRL = addr & 0x00FF;

    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;                     //对IAP_TRIG先写0x5A再写0xA5，ISP/IAP命令才会生效

    return IAP_DATA;
}

void IAP_Read_String(UINT16 addr, UINT8 * s, UINT8 len)
{
  UINT8 i;
  for (i=0; i<len; i++)
  {
    s[i] = IAP_Read_Byte(addr);
    addr++;
  }
  IAP_disable();
} 
 
void IAP_Write_Byte(UINT16 addr, UINT8 ch)
{
    IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM 字节编程命令

    IAP_ADDRH = (addr & 0xFF00)>>8;
    IAP_ADDRL = addr & 0x00FF;

    IAP_DATA = ch;

    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;                     //对IAP_TRIG先写0x5A再写0xA5，ISP/IAP命令才会生效
}

void IAP_Write_String(UINT16 addr, UINT8 * s, UINT8 len)
{
  UINT8 i;

  for (i=0; i<len; i++)
  {
    IAP_Write_Byte(addr, s[i]);
    addr++;
  }
  IAP_disable();
} 

void IAP_Erase_Sector0()
{
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM 扇区擦除命令

    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;                     //对IAP_TRIG先写0x5A再写0xA5，ISP/IAP命令才会生效
}