#ifndef _ds18b20_H
#define _ds18b20_H

#include "system.h"


/*  DS18B20时钟端口、引脚定义 */
#define DS18B20_PORT 			GPIOG  
#define DS18B20_PIN 			(GPIO_Pin_11)
#define DS18B20_PORT_RCC		RCC_APB2Periph_GPIOG


////IO操作函数											   
#define	DS18B20_DQ_OUT PGout(11) //数据端口	PG11
#define	DS18B20_DQ_IN  PGin(11)  //数据端口	PG11 
   	
u8 DS18B20_Init(void);			//初始化DS18B20
float DS18B20_GetTemperture(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(void);		//读出一个字节
u8 DS18B20_Read_Bit(void);		//读出一个位
u8 DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Reset(void);			//复位DS18B20   




#endif
