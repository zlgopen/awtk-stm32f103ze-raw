#include "tpad.h"
#include "usart.h"
#define tpad_arr_max_val 0xffff
vu16 tpad_default_val=0;
void tim5_input2_init(u32 arr,u32 pre)	//TIM5通道1输入捕获
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  //使能TIM5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //浮空输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // PA0

	TIM_TimeBaseStructure.TIM_Period= arr;  //设置了在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler= pre;//设置了用来作为 TIMx 时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1; //设置了时钟分割 如果不是0那么时钟就是72M,否则就是36M
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up; //选择了计数器模式 向上计数
	TIM_TimeBaseInit(TIM5, & TIM_TimeBaseStructure);  //初始化TIM5
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //TIM5通道1 
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising; // 上升沿捕获
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//选择IC1映射TI1上
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//设置输入捕获预分频器
	TIM_ICInitStructure.TIM_ICFilter=0x03;//使用滤波器 8个定时器时钟周期滤波
	TIM_ICInit(TIM5,&TIM_ICInitStructure);

	TIM_Cmd(TIM5,ENABLE);	 //使能TIM5外设		
}
void tpad_reset()  //触摸按键复位，配置输入口PA0为输出模式，使电容放电
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // PA0
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);

	delay_ms(5);   //延时5ms
	TIM_SetCounter(TIM5,0);	 //对TIM5计数器清零
	TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update);  //清除中断标志
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 	 //浮空输入模式	
	GPIO_Init(GPIOA, &GPIO_InitStructure);				  // PA0	
}
u16 tpad_get_val()	 //得到定时器捕获值，如果超时直接返回定时器计数值
{
	tpad_reset(); //复位tpad进行放电完全
	while(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)	 //等待溢出
	{
		if(TIM_GetCounter(TIM5)>tpad_arr_max_val-500)  //如果定时器超时，直接返回定时器计数器值
		{
			return TIM_GetCounter(TIM5); //返回TIM5的CNT值	
		}			
	}
	return TIM_GetCapture1(TIM5);  //返回TIM5通道1的捕获值		
}
u8 tpad_init(u32 psc)	//返回0：初始化成功，返回1：初始化失败
{
	u8 i,j;
	u16 buf[10],temp;
	tim5_input2_init(tpad_arr_max_val,psc-1); //以1M频率计数
	for(i=0;i<10;i++)		 //连续读取10次捕获值
	{
		buf[i]=tpad_get_val();
		delay_ms(10);		
	}
	for(i=0;i<9;i++)   //从小到大排序
	{
		for(j=i+1;j<10;j++)	
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;		
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)	//取中间6个数据的平均数
	{
		temp+=buf[i];	
	}
	tpad_default_val=temp/6;
	printf("tpad_default_val= %d\r\n",tpad_default_val); //输出打印默认的捕获时间值
	if(tpad_default_val>tpad_arr_max_val/2)return 1;
	return 0;		
}
u16 tpad_get_maxval(u8 n) //读取n次，取最大值
{
	u16 temp=0;
	u16 res=0;
	while(n--)
	{
		temp=tpad_get_val();
		if(temp>res)
		{
			res=temp;
		}
	}
	return res;		
}
//扫描触摸按键
//mode:0,不支持连续触发,按一次需松开一次再接着按下一次，1：支持连续触摸
//返回值0：没有触摸，1：有触摸
#define tpad_gate_val 100   //触摸门限值，大于tpad_default_val+tpad_gate_val才认为是有触摸
u8 tpad_scan(u8 mode)
{
	static u8 keyen=0;	 //0：可以开始检测，>0：不能开始检测
	u8 sample=3;	  //默认采样次数是3
	u8 res=0;
	u16 rval;
	if(mode)
	{	
		sample=6;	 //如果为连续按下，采样次数是6
		keyen=0;
	}
	rval=tpad_get_maxval(sample);
	if(rval>(tpad_default_val+tpad_gate_val)) //触摸  有效
	{
		if(keyen==0)
		{
			res=1;
		}
		printf("rval= %d\r\n",rval);
		keyen=3;  //至少要经过3次之后才算触摸有效			
	}
	if(keyen)keyen--;
	return res;
} 
