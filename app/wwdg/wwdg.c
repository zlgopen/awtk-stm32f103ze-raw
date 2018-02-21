#include "wwdg.h"
#include "led.h"
u8 WWDG_CNT=0x7f;
//tr是T[0-6]看门狗计数值，windowvalue是窗口值，prer是分频系数。系数为WWDG_Prescaler_1 、2、4、8
void wwdg_init(u8 tr,u8 windowvalue,u32 prer)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);  //打开窗口看门狗时钟
	WWDG_CNT=tr&WWDG_CNT;	 //tr是T[0-6]值	    初始化WWDG_CNT
	WWDG_SetWindowValue(windowvalue); //设置窗口看门狗上窗口值	 最大0x7f，最小不能为0x40
	WWDG_SetPrescaler(prer);					//设置 WWDG 预分频值
	WWDG_Enable(WWDG_CNT);		   //使能 WWDG 并装入计数器值

	WWDG_ClearFlag();		  //清除早期唤醒中断标志位
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;  //窗口看门狗中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	WWDG_EnableIT();	 //使能 WWDG 早期唤醒中断（ EWI）
}
void WWDG_IRQHandler(void)
{
	//WWDG_Enable(WWDG_CNT);	  //重新装载计数初值
	WWDG_SetCounter(WWDG_CNT);
	WWDG_ClearFlag();		  //清除早期唤醒中断标志位
	led1=~led1;
}
