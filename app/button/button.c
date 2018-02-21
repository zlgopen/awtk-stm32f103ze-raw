#include "button.h"
void button_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE, ENABLE);//使能PB\E端口时钟
	GPIO_InitStructure.GPIO_Pin = k_up;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 	 //下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);

	GPIO_InitStructure.GPIO_Pin = k_left|k_down|k_right;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //上拉输入
	GPIO_Init(GPIOE, &GPIO_InitStructure);

}
u8 keyscan(u8 mode)		  //mode=0 单次按，mode=1连续按
{
	static u8 key_up=1;
	if(mode)
	{
		key_up=1;	
	}
	if(key_up&&(K_UP==1||K_LEFT==0||K_DOWN==0||K_RIGHT==0))	
	{
		delay_ms(10);
		key_up=0;
		if(K_UP==1)
		{
			return K_UP_PRESS;
		}
		else if(K_LEFT==0)
		{
			return K_LEFT_PRESS;	
		}
		else if(K_DOWN==0)
		{
			return K_DOWN_PRESS;	
		}
		else if(K_RIGHT==0)
		{
			return K_RIGHT_PRESS;	
		}
	}
	else if(K_UP==0&&K_LEFT==1&&K_DOWN==1&&K_RIGHT==1)
	{
		key_up=1;	
	}
	return 0;
}
