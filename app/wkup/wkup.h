#ifndef _wkup_H
#define _wkup_H
#include "sys.h"
#include "delay.h"
#define WKUP_KD PAin(0)	   //PA0口

void wkup_init(void);
void Sys_Standyby(void);	  //待机模式
u8 Check_Wkup(void);	//返回1：开机 返回0：待机
void Sys_Enter_WkupStandby(void);


#endif
