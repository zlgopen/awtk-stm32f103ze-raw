#ifndef __TOUCH_H
#define __TOUCH_H

/* 包含头文件 */
#include "sys.h"
#include "delay.h"

/* 定义数据类型 */
typedef struct 
{
	uint16_t x;
	uint16_t y;
    uint16_t lcdx;
    uint16_t lcdy;
} TouchTypeDef;

typedef struct{
    uint8_t posState;   
    int8_t xOffset;
    int8_t yOffset; 
    float xFactor;
    float yFactor;
} PosTypeDef;

#define TOUCH_ADJ_OK          'Y'              //表示触摸校正参数准备好
#define TOUCH_ADJ_ADDR        6112800          //校正参数在FLASH中的首地址 6112800
/* 触摸校正因数设置 */
#define LCD_ADJX_MIN (10)                      //读取四个点的最小X值
#define LCD_ADJX_MAX (TFT_XMAX - LCD_ADJX_MIN) //读取四个点的最大X值
#define LCD_ADJY_MIN (10)                      //读取四个点的最小Y值
#define LCD_ADJY_MAX (TFT_YMAX - LCD_ADJY_MIN) //读取四个点的最大Y值

#define LCD_ADJ_X (LCD_ADJX_MAX - LCD_ADJY_MIN)//读取方框的宽度
#define LCD_ADJ_Y (LCD_ADJY_MAX - LCD_ADJY_MIN)//读取方框的高度

#define TOUCH_READ_TIMES 5     //一次读取触摸值的次数

#define TOUCH_X_CMD      0xD0  //读取X轴命令
#define TOUCH_Y_CMD      0x90  //读取Y轴命令
#define TOUCH_MAX        20    //预期差值
#define TOUCH_X_MAX      4000  //X轴最大值
#define TOUCH_X_MIN      100   //X轴最小值
#define TOUCH_Y_MAX      4000  //Y轴最大值
#define TOUCH_Y_MIN      100   //Y轴最小值

/* CS片选信号 */
#define TOUCH_CS_CLR GPIO_ResetBits(GPIOD, GPIO_Pin_6);
#define TOUCH_CS_SET GPIO_SetBits(GPIOD, GPIO_Pin_6);

/* PEN端口 */
#define TOUCH_PEN PDin(7)

extern TouchTypeDef TouchData;

void TOUCH_Init(void);
uint8_t TOUCH_Scan(void);
void TOUCH_Adjust(void);
uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue);
uint16_t TOUCH_ReadData(uint8_t cmd);
uint8_t TOUCH_ReadAdjust(uint16_t x, uint16_t y, uint16_t *xValue, uint16_t *yValue);


#endif
