#include "iwdg.h"
void iwdg_init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//使能对寄存器 IWDG_PR 和 IWDG_RLR 的写操作
	IWDG_SetPrescaler(prer);   //设置看门狗分频系数 0-7
   	IWDG_SetReload(rlr);		//设置看门狗重装载初值
	IWDG_ReloadCounter();	//按照 IWDG 重装载寄存器的值重装载 IWDG 计数器
	IWDG_Enable();			//使能 IWDG
}
void iwdg_feed()  
{
	IWDG_ReloadCounter();	
}
