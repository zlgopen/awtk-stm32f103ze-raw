#ifndef _touch_H
#define _touch_H

#include "system.h"
#include "tftlcd.h"


/* 定义数据类型 */
typedef struct 
{
	uint16_t x;
	uint16_t y;
    uint16_t lcdx;
    uint16_t lcdy;
} TouchTypeDef;

extern TouchTypeDef TouchData;

typedef struct{
    uint8_t posState;   
    int16_t xOffset;
    int16_t yOffset; 
    float xFactor;
    float yFactor;
} PosTypeDef;


#define TOUCH_ADJ_OK          'Y'              //表示触摸校正参数准备好
#define TOUCH_ADJ_ADDR        200          //校正参数在24C02中的首地址200
/* 触摸校正因数设置 */
#define LCD_ADJX_MIN (10)                      //读取四个点的最小X值
#define LCD_ADJX_MAX (tftlcd_data.width - LCD_ADJX_MIN) //读取四个点的最大X值
#define LCD_ADJY_MIN (10)                      //读取四个点的最小Y值
#define LCD_ADJY_MAX (tftlcd_data.height - LCD_ADJY_MIN) //读取四个点的最大Y值

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


//电阻屏芯片连接引脚	   
#define PEN  		PDin(7)  	//T_PEN
#define TCS  		PDout(6)  	//T_CS 



void TOUCH_Init(void);

void TOUCH_Adjust(void);
uint8_t TOUCH_Scan(void);

#endif
