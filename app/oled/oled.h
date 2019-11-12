#ifndef _oled_H
#define _oled_H



#include "system.h"
#include "stdlib.h"	



//OLED模式设置
//0:4线串行模式
//1:并行8080模式
//2:IIC模式
#define OLED_MODE 0


#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64


#if OLED_MODE==0
//OLDE-SPI4线控制管脚定义
#define OLED_CS   PDout(3)		
#define OLED_RST  PDout(4)
#define OLED_DC   PDout(5)
#define OLED_SCL  PDout(6)
#define OLED_SDA  PDout(7)
#endif


#if OLED_MODE==1
//OLDE-8080总线控制管脚定义
#define OLED_CS   PDout(3)		
#define OLED_RST  PDout(4)
#define OLED_DC   PDout(5)
#define OLED_WR   PDout(6)
#define OLED_RD   PDout(7)

#define OLED_DATA_OUT(x) GPIO_Write(GPIOC,x);//输出
#endif


#if OLED_MODE==2
//OLDE-IIC总线控制管脚定义


#endif


 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据		    						  



//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);	   							   		    
void OLED_Init(void);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size); 
void OLED_ShowFontHZ(u8 x,u8 y,u8 pos,u8 size,u8 mode);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);

#endif
