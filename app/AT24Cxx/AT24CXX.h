#ifndef _AT24CXX_H
#define _AT24CXX_H
#include "iic.h"
#define AT24C01  127
#define AT24C02  255
#define AT24C04  511
#define AT24C08  1023
#define AT24C16  2047
#define AT24C32  4095
#define AT24C64  8191
#define AT24C128 16383
#define AT24C256 32767

#define EE_TYPE  AT24C02

void AT24CXX_Init(void);//初始化IIC接口
u8 AT24CXX_Check(void);
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);
u8 AT24Cxx_ReadOneByte(u16 addr);
u16 AT24Cxx_ReadTwoByte(u16 addr);
void AT24Cxx_WriteOneByte(u16 addr,u8 dt);
void AT24Cxx_WriteTwoByte(u16 addr,u16 dt);
u16 AT24CXX_ReadLenData(u16 ReadAddr);


#endif
