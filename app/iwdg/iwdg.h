#ifndef _iwdg_H
#define _iwdg_H
#include "sys.h"
#include "delay.h"
void iwdg_init(u8 prer,u16 rlr);
void iwdg_feed(void);


#endif
