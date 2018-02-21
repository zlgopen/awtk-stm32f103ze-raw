#ifndef _button_H
#define _button_H
#include "sys.h"
#include "delay.h"
#define k_up GPIO_Pin_0	   //PA0
#define k_left GPIO_Pin_2	//PE2
#define k_down GPIO_Pin_3	//PE3
#define k_right GPIO_Pin_4	//PE4

#define K_UP GPIO_ReadInputDataBit(GPIOA,k_up)
#define K_LEFT GPIO_ReadInputDataBit(GPIOE,k_left)
#define K_DOWN GPIO_ReadInputDataBit(GPIOE,k_down)
#define K_RIGHT GPIO_ReadInputDataBit(GPIOE,k_right)

#define K_UP_PRESS 1
#define K_LEFT_PRESS 2
#define K_DOWN_PRESS 3
#define K_RIGHT_PRESS 4

void button_init(void);
u8 keyscan(u8 mode);
#endif
