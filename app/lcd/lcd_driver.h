#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

/* 包含头文件 */
#include"stm32f10x.h"

/* TFT地址结构体 */
typedef struct
{
	u16 TFT_CMD;
	u16 TFT_DATA;
} TFT_TypeDef;
/* 使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 */ 
/* 注意设置时STM32内部会右移一位对其! 111110=0X3E */			    
#define TFT_BASE        ((uint32_t)(0x6C000000 | 0x000007FE))
#define TFT             ((TFT_TypeDef *) TFT_BASE)

/* 定义屏的大小 */
#define TFT_XMAX 319		//设置TFT屏的大小
#define TFT_YMAX 479

/* 定义颜色的宏 */

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色


#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色) 

/* 定义全局变量 */
void TFT_WriteCmd(uint16_t cmd);
void TFT_WriteData(uint16_t dat);
void TFT_Init(void);
void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void TFT_ClearScreen(uint16_t color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
u16 LCD_ReadPoint(u16 x,u16 y);
#endif

