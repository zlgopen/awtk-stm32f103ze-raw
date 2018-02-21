#include "adc.h"

void adc_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); //使能ADC时钟和PA口时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //设置 ADC 时钟（ ADCCLK） 72/6=12M

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	 //模拟输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);	   //将外设 ADCx 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;   //ADC工作模式 独立模式
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; //ADC通道扫描模式，单通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; //ADC连续扫描还是单次模式，单次扫描
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //ADC触发方式 使用软件触发
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //ADC数据对其方式 使用数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel=1; //ADC规则转换通道数 1个
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1,ENABLE);  //使能ADC1
	ADC_ResetCalibration(ADC1);  // 重置指定的 ADC 的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取 ADC 重置校准寄存器的状态
	ADC_StartCalibration(ADC1);  //开始指定 ADC 的校准状态
	while(ADC_GetCalibrationStatus(ADC1));	//获取指定 ADC 的校准程序
			
}

void adc5_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); //使能ADC时钟和PA口时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //设置 ADC 时钟（ ADCCLK） 72/6=12M

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 	 //模拟输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);	   //将外设 ADCx 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;   //ADC工作模式 独立模式
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; //ADC通道扫描模式，单通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; //ADC连续扫描还是单次模式，单次扫描
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //ADC触发方式 使用软件触发
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //ADC数据对其方式 使用数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel=1; //ADC规则转换通道数 1个
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_Cmd(ADC1,ENABLE);  //使能ADC1
	ADC_ResetCalibration(ADC1);  // 重置指定的 ADC 的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取 ADC 重置校准寄存器的状态
	ADC_StartCalibration(ADC1);  //开始指定 ADC 的校准状态
	while(ADC_GetCalibrationStatus(ADC1));	//获取指定 ADC 的校准程序
			
}
u16 ADC_Getvalue(u8 ch) //获取ADC转换值	 ch:0-3(ADC_Channel_0  --  ADC_Channel_17)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);//设置ADC规则通道及采样时间
	ADC_SoftwareStartConvCmd(ADC1,ENABLE); //ADC开始软件转换
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));  //等待转换完成
	return ADC_GetConversionValue(ADC1);	 //返回最近一次转换结果			
}
u16 ADC_Get_Average(u8 ch,u8 time)  //求取读取多次通道ADC转换的平均值
{
	u8 i;
	u32 temp=0;
	for(i=0;i<time;i++)
	{
		temp+=ADC_Getvalue(ch);	
		delay_ms(5);	//间隔点时间采样
	}
	return (u16)(temp/time);		
}

