#include "spi.h"

/****************************************************************************
* Function Name  : SPI1_Config
* Description    : 初始化SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI的IO口和SPI外设打开时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* SPI的IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7上拉
/***************************************************************************/
/************************* 设置SPI的参数 ***********************************/
/***************************************************************************/
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//选择全双工SPI模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位SPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;       //时钟悬空高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      //在第二个时钟采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nss使用软件控制
	/* 选择波特率预分频为256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//从最高位开始传输
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);
}      

/****************************************************************************
* Function Name  : SPI2_Config
* Description    : 初始化SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SPI2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI的IO口和SPI外设打开时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* SPI的IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

/***************************************************************************/
/************************* 设置SPI的参数 ***********************************/
/***************************************************************************/
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//选择全双工SPI模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位SPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //时钟悬空高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;      //在第二个时钟采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nss使用软件控制
	/* 选择波特率预分频为256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//从最高位开始传输
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Cmd(SPI2, ENABLE);
	SPI_Init(SPI2, &SPI_InitStructure);
}      

/****************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : 设置SPI1的传输速度。
* Input          : 速度波特率分频
* Output         : None
* Return         : None
****************************************************************************/

void SPI1_SetSpeed(uint8_t speed)
{
	SPI1->CR1 &= 0xFFC7; 
	SPI1->CR1 |= speed;
	SPI_Cmd(SPI1, ENABLE);
	
}

/****************************************************************************
* Function Name  : SPI2_SetSpeed
* Description    : 设置SPI2的分频数，以改变SPI2的速度.
* Input          : Speed：分频数
* Output         : None
* Return         : None
****************************************************************************/

void SPI2_SetSpeed(uint8_t Speed)
{
	
	SPI2->CR1 &= 0xFFC7; 
	SPI2->CR1 |= Speed;
	SPI_Cmd(SPI2,ENABLE);
	 		
}

/****************************************************************************
* Function Name  : SPI1_WriteReadData
* Description    : 使用SPI1写入一个字节数据同时读取一个字节数据。
* Input          : dat：要写的8位数据
* Output         : None
* Return         : 读取到的8位数据
****************************************************************************/

uint8_t SPI1_WriteReadData(uint8_t dat)
{
	uint16_t i = 0;

    /* 当发送缓冲器空 */	
 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
	
    /* 发送数据 */
   	SPI_I2S_SendData(SPI1, dat);
	
	/* 等待接收缓冲器为非空 */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
 	/* 将读取到的数值返回 */
 	return SPI_I2S_ReceiveData(SPI1);		
}

/****************************************************************************
* Function Name  : SPI2_WriteReadData
* Description    : 使用SPI2写入一个字节数据同时读取一个字节数据。
* Input          : dat：写入的数据
* Output         : None
* Return         : 读取到的数据
*                * 读取失败返回0xFF
****************************************************************************/

uint8_t SPI2_WriteReadData(uint8_t dat)
{
	uint16_t i = 0;

    /* 当发送缓冲器空 */	
 	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
	
    /* 发送数据 */
   	SPI_I2S_SendData(SPI2, dat);
	
	/* 等待接收缓冲器为非空 */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	
 	/* 将读取到的数值返回 */
 	return SPI_I2S_ReceiveData(SPI2);		
}



















