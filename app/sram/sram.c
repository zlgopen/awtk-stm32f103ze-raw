#include "sram.h"


//初始化外部SRAM
void FSMC_SRAM_Init(void)
{
	FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTiming;
	GPIO_InitTypeDef GPIO_InitStructure;

	//PD,PE-FSMC_DATA 	  PF,PG-FSMC_A
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE  
	                      | RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;	  //FSMC_NE3 PG10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;	  //FSMC_NBL0-1 PE0	PE1
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5;	  //FSMC_NOE,FSMC_NWE PD4 PD5
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8  
								  | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11
								  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
								  | GPIO_Pin_15 );
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
	                               | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
								   | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  
								  | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5
								  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
								  | GPIO_Pin_15 );
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2  
								  | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5);
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	
	FSMC_NORSRAMTiming.FSMC_AddressSetupTime = 0x00; //地址建立时间为1个HCLK
	FSMC_NORSRAMTiming.FSMC_AddressHoldTime = 0x00;	//地址保持时间模式A未使用
	FSMC_NORSRAMTiming.FSMC_DataSetupTime = 0x03;  //数据保持时间为3个HCLK
	FSMC_NORSRAMTiming.FSMC_DataLatency = 0x00;
	FSMC_NORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	FSMC_NORSRAMTiming.FSMC_CLKDivision = 0x00;
	FSMC_NORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //使用模式A

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;	 //使用Bank1-3区
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//存储器写使能
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; //读写使用相同的时序
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTiming;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);		
}

//FSMC_SRAM写函数
//pbuf：写入的数据
//writeaddr：写入的数据的起始地址
//len：写入数据的长度
void FSMC_SRAM_WriteBuf(u8 *pbuf,u32 writeaddr,u32 len)
{
	while(len--)
	{
		*(u8*)(SRAM_ADDR+writeaddr)=*pbuf;
		writeaddr++;
		pbuf++;
	}		
}

//FSMC_SRAM读函数
//pbuf：保存读的数据
//readaddr：读取数据的起始地址
//len：读数据的长度
void FSMC_SRAM_ReadBuf(u8 *pbuf,u32 readaddr,u32 len)
{
	while(len--)
	{
		*pbuf=*(u8*)(SRAM_ADDR+readaddr);
		readaddr++;
		pbuf++;
	}		
}

//SRAM写测试，写一个字节
//dat：写入的数据
//wirte_addr：写入的地址
void FSMC_SRAM_Test_Write(u8 dat,u32 wirte_addr)
{
	FSMC_SRAM_WriteBuf(&dat,wirte_addr,1);			
}

//SRAM读测试，读一个字节
//read_addr：读取的地址
u8 FSMC_SRAM_Test_Read(u32 read_addr)
{
	u8 dat;
	FSMC_SRAM_ReadBuf(&dat,read_addr,1);
	return dat;			
}

