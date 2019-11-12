#include "wkup.h"

/*******************************************************************************
* 函 数 名         : Enter_Standby_Mode
* 函数功能		   : 进入待机模式
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Enter_Standby_Mode(void)
{
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//使能PWR外设时钟
	
	PWR_ClearFlag(PWR_FLAG_WU);//清除Wake-up 标志
	
	PWR_WakeUpPinCmd(ENABLE);//使能唤醒管脚	使能或者失能唤醒管脚功能
	
	
	PWR_EnterSTANDBYMode();//进入待机模式
}



