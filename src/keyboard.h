#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "mytype.h"

#define NO_KEY    0
#define PAGE_DOWN 1
#define PAGE_UP   2
#define KEY_ENTER 3
#define KEY_ESC   4

UINT8 wait_for_key(UINT8);

#endif