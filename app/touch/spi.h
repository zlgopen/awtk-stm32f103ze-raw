#ifndef __SPI_H
#define __SPI_H


#include "stm32f10x.h"


/* 定义全局变量 */
void SPI2_Config(void);
void SPI2_SetSpeed(uint8_t Speed);
uint8_t SPI2_WriteReadData(uint8_t dat);

void SPI1_Config(void);
void SPI1_SetSpeed(uint8_t speed);
uint8_t SPI1_WriteReadData(uint8_t dat);













#endif
