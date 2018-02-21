#include "dma.h"

//DMA_CHx:DMA 通道 CHx	 DMA1_Channel1-DMA1_Channel7
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量
u16 DMA1_MEM_LEN; 
void dma_init(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//开启DMA1时钟
	DMA_DeInit(DMA_CHx);  //复位DMA各寄存器值
	DMA1_MEM_LEN=cndtr;
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);	  //使能指定的DMA请求

	DMA_InitStructure.DMA_PeripheralBaseAddr =cpar; //外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar; //内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	//DMA数据传输方向  从内存到外设（目的地）
	DMA_InitStructure.DMA_BufferSize = cndtr;//	DMA缓存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//内存地址增加
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;//外设数据宽度8位
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;	//内存数据宽度8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//DMA通道中优先级
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //失能内存到内存
	DMA_Init(DMA_CHx, &DMA_InitStructure);	
	 
}
void DMA_Enable(DMA_Channel_TypeDef* DMA_CHx)  //开启DMA一次传输
{
	DMA_Cmd(DMA_CHx,DISABLE);  //使能通道DMA
	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);	//设置DMA缓存大小
	DMA_Cmd(DMA_CHx,ENABLE);  //使能通道DMA		
}


