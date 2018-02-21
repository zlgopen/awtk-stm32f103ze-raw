#include "tsensor.h"
#include "adc.h"
void T_adc_init()
{
	ADC_InitTypeDef ADC_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE); //使能ADC时钟和PA口时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  //设置 ADC 时钟（ ADCCLK） 72/6=12M

	ADC_DeInit(ADC1);	   //将外设 ADCx 的全部寄存器重设为缺省值
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;   //ADC工作模式 独立模式
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; //ADC通道扫描模式，单通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; //ADC连续扫描还是单次模式，单次扫描
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //ADC触发方式 使用软件触发
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //ADC数据对其方式 使用数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel=1; //ADC规则转换通道数 1个
	ADC_Init(ADC1,&ADC_InitStructure);

	ADC_TempSensorVrefintCmd(ENABLE);	//使能温度传感器和内部参考电压通道
	ADC_Cmd(ADC1,ENABLE);  //使能ADC1
	ADC_ResetCalibration(ADC1);  // 重置指定的 ADC 的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	//获取 ADC 重置校准寄存器的状态
	ADC_StartCalibration(ADC1);  //开始指定 ADC 的校准状态
	while(ADC_GetCalibrationStatus(ADC1));	//获取指定 ADC 的校准程序
}
int Get_Temper()	   //获取内部温度值	  
{
	u16 adc_value=0;
	double temp;
	int value;
	adc_value=ADC_Get_Average(ADC_Channel_16,20); 
	temp=adc_value*3.3/4096;
	temp=(1.43-temp)/0.0043+25;  //内部温度计算公式
	value=temp*100;
	return value;					
}

