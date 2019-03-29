#include "IAP.h"
#include "mytype.h"
#include <string.h>

void IAP_disable(void)
{
    //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    IAP_CONTR = 0;      //�ر�IAP ����
    IAP_CMD   = 0;      //������Ĵ���,ʹ����Ĵ���������,�˾�ɲ���
    IAP_TRIG  = 0;      //��������Ĵ���,ʹ������Ĵ����޴���,�˾�ɲ���
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}

UINT8 IAP_Read_Byte(UINT16 addr)
{
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM �ֽڶ�����

    IAP_ADDRH = (addr & 0xFF00)>>8;
    IAP_ADDRL = addr & 0x00FF;

    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;                     //��IAP_TRIG��д0x5A��д0xA5��ISP/IAP����Ż���Ч

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
    IAP_CONTR = ENABLE_ISP;         //�� IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM �ֽڱ������

    IAP_ADDRH = (addr & 0xFF00)>>8;
    IAP_ADDRL = addr & 0x00FF;

    IAP_DATA = ch;

    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;                     //��IAP_TRIG��д0x5A��д0xA5��ISP/IAP����Ż���Ч
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
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM ������������

    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;                     //��IAP_TRIG��д0x5A��д0xA5��ISP/IAP����Ż���Ч
}