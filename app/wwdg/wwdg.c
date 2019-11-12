#include "wwdg.h"
#include "led.h"
#include "SysTick.h"

/*******************************************************************************
* 函 数 名         : WWDG_Init
* 函数功能		   : WWDG初始化
					 窗口值是：0x5f
					 定时器计数值是：0x7f
					 预分频值是:8
					 窗口看门狗进入中断的频率计算公式是：PCLK1/(4096*2^pre)
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void WWDG_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE); //开启窗口看门狗的时钟
	WWDG_SetWindowValue(0x5f);//设置窗口值
	WWDG_SetPrescaler(WWDG_Prescaler_8);//设置分频值
	
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;//窗口中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	
	WWDG_Enable(0x7f); //使能窗口看门狗并初始化计数器值
	WWDG_ClearFlag(); //清除窗口看门狗状态标志(这一句必须加上，否则进入不了中断)
	WWDG_EnableIT(); //开启中断
	
}

/*******************************************************************************
* 函 数 名         : WWDG_IRQHandler
* 函数功能		   : WWDG中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void WWDG_IRQHandler(void)
{
	WWDG_SetCounter(0x7f); //重新赋值
	WWDG_ClearFlag(); //清除窗口看门狗状态标志
	led2=!led2;	
}
