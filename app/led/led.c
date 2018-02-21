#include "led.h"
void led_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	    //使能PC端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);			     
	GPIO_SetBits(GPIOC,GPIO_Pin_All);				
}
void led_run(u8 fx,u16 time)	   //设定跑马灯的方向  和时间
{
	static u8 i;
	if(fx==0)	//向右跑
	{
		GPIO_Write(GPIOC,~(0x01<<i++));
		delay_ms(time);
		if(i==8)i=0;		
	}
	else	  //向左跑
	{
		GPIO_Write(GPIOC,~(0x80>>i++));
		delay_ms(time);
		if(i==8)i=0;	
	}	
}

