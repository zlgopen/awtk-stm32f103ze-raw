#ifndef _wwdg_H
#define _wwdg_H
#include "sys.h"
#include "delay.h"
void wwdg_init(u8 tr,u8 windowvalue,u32 prer);
void WWDG_IRQHandler(void);

#endif
