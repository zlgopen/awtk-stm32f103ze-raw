#include "tim.h"
#include "sys.h"
#include "delay.h"

u8 TIM5CH1_CAPTURE_STA=0;
u16 TIM5CH1_CAPTURE_VAL=0;

extern u16 xval,yval,Scores,Level_Up,Level,Vel;
extern u8 k,m,k1;
extern u16 xc[4],yc[4];
void tim3_pwm_init(u32 arr,u32 pre)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;	
	TIM_OCInitTypeDef 	TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //使能TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOC, ENABLE);  //使能端口C时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE); //将TIM3——CH2完全映射到PC7口

	TIM_TimeBaseStructure.TIM_Period= arr;  //设置了在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler= pre;//设置了用来作为 TIMx 时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1; //设置了时钟分割 如果不是0那么时钟就是72M,否则就是36M
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up; //选择了计数器模式 向上计数
	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);  //初始化TIM3
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;		//选择 PWM 模式 2
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //比较输出模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //输出极性高
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);	//TIM3 在 CCR2 上的预装载寄存器使能
	TIM_Cmd(TIM3,ENABLE);	 //使能TIM3外设

}
void TIM3_Init(u32 arr,u32 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
void tim5_input1_init(u32 arr,u32 pre)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  //使能TIM5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);  //使能端口A时钟
	
	TIM_TimeBaseStructure.TIM_Period= arr;  //设置了在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler= pre;//设置了用来作为 TIMx 时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1; //设置了时钟分割 如果不是0那么时钟就是72M,否则就是36M
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up; //选择了计数器模式 向上计数
	TIM_TimeBaseInit(TIM5, & TIM_TimeBaseStructure);  //初始化TIM5
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //TIM5通道1 
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; // 上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//选择IC1映射TI1上
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//设置输入捕获预分频器
	TIM_ICInitStructure.TIM_ICFilter=0x00;//不使用滤波器
	TIM_ICInit(TIM5,&TIM_ICInitStructure);

	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);	//开启定时器更新中断和输入捕获中断

	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;  //定时器5
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM5,ENABLE);	 //使能TIM5外设		
}
void TIM5_IRQHandler(void)
{
	if((TIM5CH1_CAPTURE_STA&0x80)==0)	   //没有成功捕获
	{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)	  //发生溢出中断
		{
			if((TIM5CH1_CAPTURE_STA&0x40)==1)	//成功捕获高电平
			{
				if((TIM5CH1_CAPTURE_STA&0x3f)==0x3f) //高电平时间过长
				{	
					TIM5CH1_CAPTURE_STA|=0x80; //强制成功捕获一次
					TIM5CH1_CAPTURE_VAL=0xffff;		
				}
				else
				{			   
					TIM5CH1_CAPTURE_STA++;	
				}			
			}		
		}
		if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)	//发生捕获中断
		{
			if(TIM5CH1_CAPTURE_STA&0x40)  //捕获到一个下降沿
			{
				TIM5CH1_CAPTURE_STA|=0x80;  //标志最高位1，成功捕获一次
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);	//获取TIM5通道1捕获值
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);//设置上升沿捕获		
			}
			else
			{
				TIM5CH1_CAPTURE_STA=0;
				TIM5CH1_CAPTURE_VAL=0;
				TIM_SetCounter(TIM5,0);	 //清空TIM5计数器
				TIM5CH1_CAPTURE_STA|=0x40;	//标记捕获到了上升沿
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);//设置下降沿捕获		
			}
		}				
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update|TIM_IT_CC1);	 //清除更新和捕获中断标志	
}
/*void TIM3_IRQHandler(void)
{
	u8 flag=0,i;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

	}
}  */



