#ifndef _tpad_H
#define _tpad_H
#include "sys.h"
#include "delay.h"
extern vu16 tpad_default_val; 
u8 tpad_init(u32 psc);	//返回0：初始化成功，返回1：初始化失败
u8 tpad_scan(u8 mode);

#endif
