#ifndef _rs485_H
#define _rs485_H

#include "system.h"


//模式控制
#define RS485_TX_EN		PGout(3)	//485模式控制.0,接收;1,发送.
														 
void RS485_Init(u32 bound);


#endif
