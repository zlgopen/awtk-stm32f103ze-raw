#include "wkup.h"
#include "led.h"
#include "gui.h"
void wkup_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);//打开端口A和复用时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 //下拉输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);  //要加上这条语句，将PA0口拉低

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

	EXTI_InitStructure.EXTI_Line=EXTI_Line0;		 //中断线路0
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;  //中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;  //使能
	EXTI_Init(&EXTI_InitStructure);	 //初始化

	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;  //外部中断线0通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if(Check_Wkup()==0)	   //不是开机进入待机模式
	{
		GUI_Show12ASCII(10,150,"Enter Standby Mode...",GREEN,BLACK);
		Sys_Standyby();  
	}

}

void Sys_Standyby()	  //待机模式
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE); //打开电源时钟
	PWR_WakeUpPinCmd(ENABLE);//使能唤醒管脚WAKE_UP	
	PWR_EnterSTANDBYMode(); //进入待机模式
}
void Sys_Enter_WkupStandby()
{
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//复位所有IO口
	Sys_Standyby();	
}
u8 Check_Wkup()	//返回1：开机 返回0：待机
{
	u8 t=0;
	while(1)
	{
		if(WKUP_KD)
		{
			t++;
			delay_ms(20);
			if(t>=100)
			{
				led0=0;	   //亮
				return 1;  //长按3秒表示开机		
			}
		}
		else
		{
			led0=1;	  //灭
			return 0;  //不足3秒表示进入待机
		}	
	}	
}
//如果使用待机唤醒，在exti.c文件内的中断0函数必须注释，否则编译会报错
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)	
	{	
		delay_ms(10);
		if(Check_Wkup())   //是否关机？
		{
			Sys_Enter_WkupStandby();
		}	
	}
	EXTI_ClearITPendingBit(EXTI_Line0);		
}

