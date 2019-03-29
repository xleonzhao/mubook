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
         |  ���ϵ��� �߼�������� ֮  ��ȡ�ļ�����  |
         +------------------------------------------+

  ��Դ���Ȩ�� ���� ȫȨ���У��������ã�������������֪
        �Ͻ�����������ҵĿ�ģ�Υ�߱ؾ�������Ը�
         ���ϵ��� 
             ->��Ʒ��վ http://www.znmcu.cn/
             ->��Ʒ���� http://shop.znmcu.cn/
             ->��Ʒ��ѯ QQ :987582714 
			            MSN:yzn07@126.com
	                    WW :yzn07
  ����˵�������������znFAT�Ķ�ȡ�ļ����ݺ�������\test.txt������
            ���ж�ȡ���������������ݷ������ڡ�
  ע��SD����ؽ̳���μ� 
     �����ϵķ��Ƭ��(51)���߼���Χƪ ֮ ��SD����II����				  
**************************************************************/

unsigned char Dev_No; //�豸��
struct znFAT_Init_Arg *pArg; //����ָ���ļ�ϵͳ�������ϵ�ָ��
struct znFAT_Init_Arg Init_Arg_SDCARD; //�ļ�ϵͳ�������ϣ����ڼ�¼�ļ�ϵͳ����Ҫ����

BOOK_INFO lastbook;

window TFT_Win; /* this variable has to be defined here. why? */

void init_all()
{
  P1=0x00; //lit all lights

#ifdef debug
  /* init serial port for debug */
  UART_Init(); //���ڳ�ʼ��
#endif

  /* init SD device */
  znFAT_Device_Init(); //�洢�豸��ʼ��
  pArg=&Init_Arg_SDCARD;	//ָ��ָ��SD���ļ�ϵͳ�������ϣ�znFAT������������л�ȡ����
  Dev_No=SDCARD;	//�豸��ΪSDCARD��znFAT���մ��豸��ѡ��洢�豸����

  /* init znFAT file system */ 
  znFAT_Init();	//�ļ�ϵͳ��ʼ��
#ifdef debug
    UART_Put_Inf("�ܴ洢������(�ֽ�)",pArg->Total_Size);//�Ӵ���������ֲ�����Ϣ
    UART_Put_Inf("ÿ�����ֽڣ�(��)",pArg->BytesPerSector);
    UART_Put_Inf("ÿ��������(��)",pArg->SectorsPerClust);
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