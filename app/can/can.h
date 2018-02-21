#ifndef _can_H
#define _can_H

#include "sys.h"
#include "delay.h"

#define CAN_RX0_INT_ENABLE 0   //不使用中断

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);
u8 Can_Send_Msg(u8* msg,u8 len);  //can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)
u8 Can_Receive_Msg(u8 *buf);   //can口接收数据查询

#endif
