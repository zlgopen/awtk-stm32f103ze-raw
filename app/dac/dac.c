#include "dac.h"

void dac1_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef	 DAC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //打开端口A PA4时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);  //打开DAC时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		//PA4	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	 //模拟输入模式 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);	 //拉高
	
	DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;	  //不使用触发功能
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;	 //不使用波形发生器
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;	//使用波形发生器才有效
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;	//关闭输出缓存
	DAC_Init(DAC_Channel_1,&DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1,ENABLE);	//使能DAC通道1
	DAC_SetChannel1Data(DAC_Align_12b_R,0); //设置DAC数据右对齐和初始化DAC值
}
void Set_Channel_val(u16 dat)  //设置DAC通道数据 0-3300（0-3.3V）
{
	float temp;
	u16 val=0;
	temp=dat/1000;
	val=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,val); 		
}
