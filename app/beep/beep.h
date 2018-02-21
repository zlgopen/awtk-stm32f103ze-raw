#ifndef _beep_H
#define _beep_H
#include "sys.h"
#include "delay.h"
#define beep GPIO_Pin_5	   //PB5
#define beep_io GPIOB	//PB¶Ë¿Ú
#define beepH GPIO_SetBits(beep_io,beep)
#define beepL GPIO_ResetBits(beep_io,beep)
void beep_init(void);
void beep_sound(void);

#endif
