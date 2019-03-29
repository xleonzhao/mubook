#ifndef __BOOK_H
#define __BOOK_H

#include "mytype.h"

#define BOOK_NAME_LEN	13 //naming follows DOS 8.3 format with a backslash at the beginning.
#define BUFFER_SIZE 128
#define INDEXBOOK "\\index.txt"
#define LASTBOOK_NAME 0
#define LASTBOOK_OFFSET LASTBOOK_NAME+BOOK_NAME_LEN+1

static UINT8 code BOOK_DELIMIT[4]={0x0d,0x0a,0x0d,0x0a};

typedef struct _book_info {
	UINT8 book_name[BOOK_NAME_LEN];
	UINT32 offset;
  UINT8 is_index;
} BOOK_INFO;

void read_book(BOOK_INFO *);
void load_index(BOOK_INFO *);
void read_last_book(BOOK_INFO *);
void write_last_book(BOOK_INFO *);

#endif