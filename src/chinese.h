#ifndef __CHINESE_H
#define __CHINESE_H

#include "mytype.h"

#define HZK16_FILE "\\HZK16"
#define CCHAR_WIDTH  16
#define CCHAR_HEIGHT 16
#define CCHAR_BYTES  32
#define LINE_HEIGHT  16

void init_chinese();
void display_chinese_char(UINT8 cx, UINT8 cy, UINT8 charH, UINT8 charL);

#endif