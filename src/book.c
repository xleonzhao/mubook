#include "znFAT.h"
#include "uart.h"
#include "mytype.h"
#include "book.h"
#include "debug.h"
#include "chinese.h"
#include "english.h"
#include "myfun.h"
#include "tft.h"
#include "keyboard.h"
#include "string.h"
#include "IAP.H"

UINT8 read_auto_mode = 0;
UINT8 buf[BUFFER_SIZE];

void display_page(UINT8 * string, UINT32 len, UINT32 * dlen)
{
  UINT8 i=0, j=0, k=0;
  UINT8 cx, cy, px=START_X, py=START_Y;

  // clean screen
  TFT_Clr(0x0000);

  *dlen = 0;
  cx=px;
  cy=py;

  for(i=0;i<len;i++)
  {
	  if (string[i] & 0x80 ) /* highest bit is 1, it is a chinese char */
	  {
	    if ((i+1) < len) 
	    {
	      display_chinese_char(cx, cy, string[i], string[i+1]);
        (*dlen)+=2;
		    i++;
        /* move to next cchar position */
        px+=CCHAR_WIDTH;
	    } else { /* now we have a single chinese char which is two bytes */
	             /* and 2nd byte is not in the buffer. leave it to  */
			         /* be displayed next time. */
        break;
	    }
	  } else {
      (*dlen)++;
	    switch(string[i]) 
	    {
	    	case 0x0d: 
	        px = START_X;
		      break;
		    case 0x0a:
 	        py += LINE_HEIGHT;
		      break;
		    default:
	        display_english_char(cx, cy, string[i]);
          px+=ECHAR_WIDTH;
		      break;
	    }
	  }

    if (px >= MAX_X - (CCHAR_WIDTH/2)) { /* make sure able to print a chinese char at px */
	    /* already move to rightmost of tft, move down by one cchar */
	    py+=LINE_HEIGHT;
	    px=START_X;
	  }
	  if( py >= MAX_Y) {
	    break;
	  }

	  /* adjust cursor */
    cx = px;
    cy = py;
  }
}

void write_last_book(BOOK_INFO *book)
{
  IAP_Erase_Sector0(); //has to erase the whole sector first, then can write.

  IAP_Write_String(LASTBOOK_NAME, book->book_name, strlen(book->book_name)+1); //important to write the ending '\0' as well.
  IAP_Write_String(LASTBOOK_OFFSET, (UINT8 *)&(book->offset), sizeof(UINT32));
}

void read_last_book(BOOK_INFO *book)
{
  UINT8 len;
  UINT32 offset;

  IAP_Read_String(LASTBOOK_NAME, buf, BOOK_NAME_LEN);
  IAP_Read_String(LASTBOOK_OFFSET, (UINT8 *)&offset, sizeof(UINT32));

  len=strlen(buf);
  strncpy(book->book_name, buf, len<BOOK_NAME_LEN?len:BOOK_NAME_LEN);
  book->offset = offset;
  if(strncmp(book->book_name, INDEXBOOK, BOOK_NAME_LEN) == 0) {
    book->is_index = 1;
  } else {
    book->is_index = 0;
  } 
#ifdef debug
    UART_Send_Str("last book is：");
    UART_Send_Str(book->book_name);
    UART_Send_Enter();
    UART_Put_Inf("文件偏移量(字节)：", book->offset);
#endif
}

void load_index(BOOK_INFO *book)
{
  strcpy(book->book_name, INDEXBOOK);
  book->offset = 0;
  book->is_index = 1;
}

void read_book(BOOK_INFO *book)
{
  struct FileInfoStruct FileInfo;	//文件参数集合
  UINT8 i=0,key=NO_KEY,pos=0;
  UINT32 len=0, disp_len=0;
  UINT8 * strp;

  /* open the book */
  if(znFAT_Open_File(&FileInfo, book->book_name, 0, 1))
  {  
#ifdef debug
    UART_Send_Str("打开文件失败\n");
    UART_Send_Str("文件名为：");
    UART_Send_Str(book->book_name);
    UART_Put_Inf("文件偏移量(字节)：", book->offset);
    UART_Send_Enter();
#endif
    load_index(book);
    return;
  }

#ifdef debug
    UART_Send_Str("打开文件成功\n"); //从串口输出文件参数信息
    UART_Send_Str("文件名为：");
    UART_Send_Str(FileInfo.FileName);
    UART_Send_Enter();
    UART_Put_Inf("文件大小(字节)：",FileInfo.FileSize);
    UART_Put_Inf("文件当前偏移量(字节)：",FileInfo.FileCurOffset);

    UART_Send_Str("文件创建时间：\n");
    UART_Put_Num(FileInfo.FileCreateDate.year);UART_Send_Str("年");
    UART_Put_Num(FileInfo.FileCreateDate.month);UART_Send_Str("月");
    UART_Put_Num(FileInfo.FileCreateDate.day);UART_Send_Str("日");
    UART_Put_Num(FileInfo.FileCreateTime.hour);UART_Send_Str("时");
    UART_Put_Num(FileInfo.FileCreateTime.min);UART_Send_Str("分");
    UART_Put_Num(FileInfo.FileCreateTime.sec);UART_Send_Str("秒");
    UART_Send_Enter();
#endif

  while(1) {
    /* read the 128 bytes from the file */
    if(len=znFAT_Read_File(&FileInfo, book->offset, BUFFER_SIZE, buf)) //读取文件数据到数据缓冲区
    {
      if(book->is_index) {
        strp = strstr(buf, BOOK_DELIMIT);
        if(strp != NULL) {
          pos = strp - buf;
	        display_page(buf, pos, &disp_len);
          disp_len+=strlen(BOOK_DELIMIT); //skip delimit
        } else {
#ifdef debug
        UART_Send_Str("cannot find book delimit!\n");	
        UART_Send_Str(BOOK_DELIMIT);
        UART_Send_Enter();
	      display_page(buf, len, &disp_len);
#endif 
        }         
      } else {
	      display_page(buf, len, &disp_len);
      }

#ifdef debug
        UART_Send_Str("读取文件成功\n");	
        UART_Put_Inf("共读出字节：",len);
        UART_Put_Inf("共显示字节：",disp_len);
        UART_Put_Inf("文件当前偏移量(字节)：",FileInfo.FileCurOffset);
        UART_Send_Str("读到的内容是：\n");
	
        for(i=0;i<len;i++)	
        {
          UART_Send_Byte(buf[i]);
        }     
#endif
	  } else {
#ifdef debug
      UART_Send_Str("读取文件失败\n");
#endif
      load_index(book);
	    return;
	  }

    if (!book->is_index) {
      write_last_book(book); //remember where we are
    }

    key = wait_for_key(read_auto_mode);

    if(key == KEY_ENTER) {
      if(book->is_index) {
        i = strpos(buf, '\\'); //find begining of filename
        pos = strpos(buf+i, 0x0d); //find ending of filename
        if(pos > 0 && pos <= BOOK_NAME_LEN)
        {
          strncpy(book->book_name, buf, pos);
          book->book_name[pos] = 0;
          book->offset = 0;   //FIXME: new book, starting from beginning.
          book->is_index = 0;
#ifdef debug
        UART_Send_Str("change book to ");
        UART_Send_Str(book->book_name);
        UART_Send_Enter();
#endif            
          return;
        }
	    } else {
        read_auto_mode = !read_auto_mode;
        key = PAGE_DOWN;
      }
    }

    switch(key)
    {
      case PAGE_DOWN:
	      if (len == BUFFER_SIZE || (book->is_index && len > 0)) {
          book->offset += disp_len;
	      } else {
#ifdef debug
          /* FIXME: it may be the case that no content from file, but there */
          /* may be still chars can be displayed. */
          UART_Send_Str("End of the book.\n");
#endif
	      }
        break;
	    case PAGE_UP:
        /* FIXME: cannot remember how many bytes displayed last time, so be it. */
	      if(book->offset <= BUFFER_SIZE) {
	        book->offset = 0;
#ifdef debug
          UART_Send_Str("Beginning of the book.\n");
#endif
        } else {
	        book->offset -= BUFFER_SIZE;
	      }
	      break;
      case KEY_ESC:
       load_index(book);
       return;
	  }
  }
}