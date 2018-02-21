#ifndef _led_H
#define _led_H
#include "sys.h"
#include "delay.h"
void led_init(void);
void led_run(u8 fx,u16 time);	   //设定跑马灯的方向  和时间
#define ledH GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define ledL GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define led0 PCout(0)
#define led1 PCout(1)
#define led2 PCout(2)
#define led3 PCout(3)	
#endif
