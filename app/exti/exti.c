#include "exti.h"
#include "button.h"
#include "led.h"
void exti_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //端口复用时钟

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;		 //中断线路0
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  //使能
	EXTI_Init(&EXTI_InitStructure);	 //初始化

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
	EXTI_InitStructure.EXTI_Line=EXTI_Line2;		 //中断线路2
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  //使能
	EXTI_Init(&EXTI_InitStructure);	 //初始化

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
	EXTI_InitStructure.EXTI_Line=EXTI_Line3;		 //中断线路3
	EXTI_Init(&EXTI_InitStructure);	 //初始化

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line=EXTI_Line4;		 //中断线路4
	EXTI_Init(&EXTI_InitStructure);	 //初始化
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;  //外部中断线0通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;  //外部中断线2通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;  //外部中断线3通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;  //外部中断线4通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
 /*   //如果使用待机唤醒，这个就要注释，因为待机唤醒也使用了中断0函数
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)	
	{	
		if(K_UP==1)
		{
			delay_ms(10);
			if(K_UP==1)
			{
				led0=~led0;	
			}
			while(K_UP);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line0);		
}*/
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)	
	{	
		if(K_LEFT==0)
		{
			delay_ms(10);
			if(K_LEFT==0)
			{
				led1=~led1;	
			}
			while(!K_LEFT);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2);			
}
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)	
	{	
		if(K_DOWN==0)
		{
			delay_ms(10);
			if(K_DOWN==0)
			{
				led2=~led2;	
			}
			while(!K_DOWN);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line3);			
}
void EXTI4_IRQHandler(void)
{	
	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)	
	{	
		if(K_RIGHT==0)
		{
			delay_ms(10);
			if(K_RIGHT==0)
			{
				led3=~led3;	
			}
			while(!K_RIGHT);
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line4);				
}
