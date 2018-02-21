#ifndef _exti_H
#define _exti_H
#include "stm32f10x.h"
void exti_init(void);
void EXTI0_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);

#endif
