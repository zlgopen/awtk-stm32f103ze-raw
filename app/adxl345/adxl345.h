#ifndef _adxl345_H
#define _adxl345_H
#include "sys.h"
#include "delay.h"


#define DEVICE_ID		0X00 	//器件ID,0XE5
#define THRESH_TAP		0X1D   	//敲击阀值
#define OFSX			0X1E
#define OFSY			0X1F
#define OFSZ			0X20
#define DUR				0X21
#define Latent			0X22
#define Window  		0X23 
#define THRESH_ACK		0X24
#define THRESH_INACT	0X25 
#define TIME_INACT		0X26
#define ACT_INACT_CTL	0X27	 
#define THRESH_FF		0X28	
#define TIME_FF			0X29 
#define TAP_AXES		0X2A  
#define ACT_TAP_STATUS  0X2B 
#define BW_RATE			0X2C 
#define POWER_CTL		0X2D 

#define INT_ENABLE		0X2E
#define INT_MAP			0X2F
#define INT_SOURCE  	0X30
#define DATA_FORMAT	    0X31
#define DATA_X0			0X32
#define DATA_X1			0X33
#define DATA_Y0			0X34
#define DATA_Y1			0X35
#define DATA_Z0			0X36
#define DATA_Z1			0X37
#define FIFO_CTL		0X38
#define FIFO_STATUS		0X39


//0X0B TO OX1F Factory Reserved	 
//如果ALT ADDRESS脚(12脚)接地,ADXL地址为0X53(不包含最低位).
//如果接V3.3,则ADXL地址为0X1D(不包含最低位).
//因为开发板接V3.3,所以转为读写地址后,为0X3B和0X3A(如果接GND,则为0XA7和0XA6)  
#define ADXL_READ    0X3B
#define ADXL_WRITE   0X3A

u8 ADXL345_Init(void); 								//初始化ADXL345
void ADXL345_WR_Reg(u8 addr,u8 val);				//写ADXL345寄存器
u8 ADXL345_RD_Reg(u8 addr);							//读ADXL345寄存器
void ADXL345_RD_XYZ(short *x,short *y,short *z);	//读取一次值
void ADXL345_RD_Avval(short *x,short *y,short *z);	//读取平均值
void ADXL345_AUTO_Adjust(char *xval,char *yval,char *zval);//自动校准
void ADXL345_Read_Average(short *x,short *y,short *z,u8 times);//连续读取times次,取平均
short ADXL345_Get_Angle(float x,float y,float z,u8 dir);


#endif
