#include "flash.h"
#include "spi.h"
//#include "malloc.h"

/* 声明内部引用函数 */
static void FLASH_SectorErase(uint32_t eraseAddr);
static void FLASH_WriteEnable(void);
static uint8_t FLASH_CheckBusy(void);
static void FLASH_WritePage(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum);

/****************************************************************************
* Function Name  : FLASH_Init
* Description    : 初始化FLASH的IO和SPI.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

uint16_t FLASH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);
	
	/* SD_CS PG14/ FLASH_CS PG13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_14 | GPIO_Pin_13);
	
	/* ENC28J60_CS PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12); 

    /* 初始化SPI2 */
    SPI2_Config();

    /* 初始化FLASH要读取芯片ID一下 */
	return(FLASH_ReadID());   
}

/****************************************************************************
* Function Name  : FLASH_ReadID
* Description    : 读取FLASH的型号ID。
* Input          : None
* Output         : None
* Return         : 读取到的ID数据
****************************************************************************/

uint16_t FLASH_ReadID(void)
{
	uint16_t id = 0;
	
	FLASH_CS_CLR;            //打开片选
	
	SPI2_WriteReadData(0x90);//发送读取ID命令	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00);

	id |= SPI2_WriteReadData(0xFF) << 8; //读取16位ID 
	id |= SPI2_WriteReadData(0xFF);
		 
	FLASH_CS_SET;			 //关闭片选   
	return id;	
}

/****************************************************************************
* Function Name  : FLASH_CheckBusy
* Description    : 检测FLASH是否忙碌.
* Input          : None
* Output         : None
* Return         : 0：不忙碌。0xFF：等待超时。
****************************************************************************/

static uint8_t FLASH_CheckBusy(void)
{
	uint8_t statusValue;
	uint32_t timeCount = 0;
	
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF)				   //等待超时
		{
			return 0xFF;
		}
		FLASH_CS_CLR;                              //使能器件
		   
		SPI2_WriteReadData(EN25X_ReadStatusReg);   //发送读取状态寄存器命令    
		statusValue = SPI2_WriteReadData(0xFF);    //读取一个字节  
	
		FLASH_CS_SET;                              //取消片选
	}
	while((statusValue & 0x01) == 0x01);    	   // 等待BUSY位清空

	return 0; 
}

/****************************************************************************
* Function Name  : FLASH_ReadData
* Description    : 读取FLASH的数据.
* Input          : readBuff：读取缓存器
*                * readAddr：读取地址
*                * readByteNum：读取数据长度
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ReadData(uint8_t *readBuff, uint32_t readAddr, uint16_t readByteNum)
{
    SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
    FLASH_CheckBusy();				      //确定FLASH是否处于忙状态
	FLASH_CS_CLR;    //打开片选

	/* 写读取命令 */
	SPI2_WriteReadData(EN25X_ReadData);

	/* 发送24位读取地址 */
	SPI2_WriteReadData(readAddr >> 16);
	SPI2_WriteReadData(readAddr >> 8);
	SPI2_WriteReadData(readAddr);

	/* 读取数据 */
	while(readByteNum--)
	{
		*readBuff = SPI2_WriteReadData(0xFF);
		readBuff++;
	}

	FLASH_CS_SET;   //关闭片选
}

/****************************************************************************
* Function Name  : FLASH_WriteEnable
* Description    : 写使能
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void FLASH_WriteEnable(void)
{
	FLASH_CS_CLR;                               //使能器件  
	 
    SPI2_WriteReadData(EN25X_WriteEnable);       //发送写使能 
	 
	FLASH_CS_SET;                               //取消片选     	      	
}

/****************************************************************************
* Function Name  : FLASH_SectorErase
* Description    : 扇区擦除
* Input          : eraseAddr：扇区地址
* Output         : None
* Return         : None
****************************************************************************/

static void FLASH_SectorErase(uint32_t eraseAddr)
{		
	FLASH_CheckBusy();	 //确定FLASH是否处于忙状态
    FLASH_WriteEnable(); //开启写使能

	FLASH_CS_CLR;        //使能器件

	SPI2_WriteReadData(EN25X_SectorErase); //发送命令

	SPI2_WriteReadData(eraseAddr >> 16);  //发送24位地址
	SPI2_WriteReadData(eraseAddr >> 8);
	SPI2_WriteReadData(eraseAddr);

	FLASH_CS_SET;                         //取消片选     	      	 
}

/****************************************************************************
* Function Name  : FLASH_ChipErase
* Description    : 整片擦除
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ChipErase(void)
{
	FLASH_CheckBusy();	 //确定FLASH是否处于忙状态
	FLASH_WriteEnable(); //开启写使能

	FLASH_CS_CLR;        //使能器件

	SPI2_WriteReadData(EN25X_ChipErase); //发送命令

	FLASH_CS_SET;                         //取消片选
}

/****************************************************************************
* Function Name  : FLASH_WritePage
* Description    : 向空的FLASH里面写数据（注意是空的！）
* Input          : writeBuff：写缓存器
*                * writeAddr：写入地址
*                * writeByteNum：写入数据长度
* Output         : None
* Return         : None
****************************************************************************/

static void FLASH_WritePage(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum)
{
	uint16_t byteNum, i;

	byteNum = writeAddr % 256;	  
	byteNum = 256 - byteNum;		  //求出首页剩余地址
	
	if(writeByteNum <= byteNum)	      //当写入字节少于首页剩余地址
	{
		byteNum = writeByteNum;
	}
				
	/* 开始写入 */
	while(1)
	{  
		/* 写入数据 */
		FLASH_CheckBusy();				      //确定FLASH是否处于忙状态
		FLASH_WriteEnable();		          //开启写使能
 
		FLASH_CS_CLR;                         //使能器件 
		
		SPI2_WriteReadData(EN25X_PageProgram); //发送写命令
	
		SPI2_WriteReadData(writeAddr >> 16);  //发送24位读取地址
		SPI2_WriteReadData(writeAddr >> 8);
		SPI2_WriteReadData(writeAddr);

	    for(i=0; i<byteNum; i++)				  //循环写数
		{
			SPI2_WriteReadData(*writeBuff);
			writeBuff++;	
		}

		FLASH_CS_SET;                         //取消片选 

		/* 判断是否写完 */
		if(writeByteNum == byteNum) //如果写入字节数等于剩余字节数表示写入完成	
		{
			break;
		}
		else	                    //如果未写入完成
		{
			writeAddr += byteNum;					 //写入地址偏移
			writeByteNum = writeByteNum - byteNum;	 //求出剩余字节数

			if(writeByteNum >= 256)	//如果剩余字节数大于256，那么一次写入一页
			{
				byteNum = 256;	
			}
			else					//如果剩余字节数小于256，那么一次全部写完
			{
				byteNum = writeByteNum;
			}
		}
	} 	
}

/****************************************************************************
* Function Name  : FLASH_WriteData
* Description    : 向FLASH里面写数据（FLASH可以不为空）
* Input          : writeBuff：写缓存器
*                * writeAddr：写入地址
*                * writeByteNum：写入数据长度
* Output         : None
* Return         : None
****************************************************************************/
#ifndef __MALLOC_H
static uint8_t flashBuff[4096]; //读取缓存器（不知道为什么不能放到子函数里面。）
#endif

void FLASH_WriteData(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum)
{
	
	uint16_t sectorAddr, byteAddr, byte, i;

#ifdef __MALLOC_H	
    uint8_t *p;
    p = malloc(4096);                  //开辟一个内存块
    if(p == 0)
    {
        return;
    }
#endif
    SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	sectorAddr = writeAddr / 4096;	   //扇区地址
	byteAddr = writeAddr % 4096;	   //扇区偏移地址
	byte = 4096 - byteAddr;            //扇区剩余地址

	if(writeByteNum < byte)	   //如果写入字节小于扇区剩余空间(小于一个扇区)
	{
		byte = writeByteNum;
	}
	while(1)
	{

#ifdef __MALLOC_H
        FLASH_ReadData(p, sectorAddr*4096, 4096);         //读取一个扇区的数据
#else
		FLASH_ReadData(flashBuff, sectorAddr*4096, 4096); //读取一个扇区的数据
#endif
		for(i=0; i<byte; i++)	  		//检测要写入的地址是不是空的
		{
#ifdef __MALLOC_H
            if(p[byteAddr + i] != 0xFF)
#else       
			if(flashBuff[byteAddr + i] != 0xFF)	//如果不是
#endif
			{
				break;
			} 		
		}

		if(i < byte) //如果写入地址里面有数据
		{
			FLASH_SectorErase(sectorAddr*4096); //擦除这个扇区
			
			for(i=0; i<byte; i++)				//复制数据
			{
#ifdef __MALLOC_H
                p[byteAddr + i] = writeBuff[i];
#else       
				flashBuff[byteAddr + i] = writeBuff[i];
#endif

			}
#ifdef __MALLOC_H
			FLASH_WritePage(p, sectorAddr*4096, 4096);
#else       
			FLASH_WritePage(flashBuff, sectorAddr*4096, 4096);
#endif		
		}
		else		 //要写入的地址内无数据
		{
			FLASH_WritePage(writeBuff, writeAddr, byte);			
		}

		/* 判断是否写完 */
		if(byte == writeByteNum)		//如果写入字节数等于剩余字节数表示写入完成
		{
			break;
		}
		else                        //如果未写完
		{
			writeBuff += byte;		//数据指针偏移
			writeAddr += byte;		//求出写入地址偏移
			writeByteNum -= byte;   //求出写入数据剩余数
			sectorAddr++;			//扇区地址+1
			byteAddr = 0;           //扇区偏移地址为0

			if(writeByteNum >= 4096)
			{
				byte = 4096;	
			}
			else
			{
				byte = writeByteNum;
			}		
		}
	}
#ifdef __MALLOC_H	
    free(p);
#endif    	 				
}

