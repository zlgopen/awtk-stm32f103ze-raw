#include "stm32_flash.h"


//STM32内部Flash初始化
void STM32_Flash_Init(void)
{
	FLASH_Unlock();	 //STM32 Flash解锁
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位
	
	FLASH_Lock();//锁定			
}

//STM32Flash写数据 一次写16位数据
//write_addr：写入的地址
//pdat：写入的数据
//len：写入的长度
void STM32_Flash_Write(u32 write_addr,u16 *pdat,u16 len)   
{
	u16 i;
	FLASH_Unlock();	 //STM32 Flash解锁
	FLASH_ErasePage(flash_adr);//要擦出页的起始地址
	for(i=0;i<len;i++)
	{
		FLASH_ProgramHalfWord(write_addr,pdat[i]);   //写入半字函数
		write_addr+=2;
	}
	FLASH_Lock();//锁定			
}

//STM32Flash读数据 
//read_addr：写入的地址
//pdat：写入的数据
//len：写入的长度
void STM32_Flash_Read(u32 read_addr,u16 *pdat,u16 len) 
{ 	
	u16 i;
	FLASH_Unlock();	 //STM32 Flash解锁
	for(i=0;i<len;i++)
	{
		pdat[i]=STM32_Flash_HalfRead(read_addr);
		read_addr+=2;
	}
	FLASH_Lock();//锁定		
}

u16 STM32_Flash_HalfRead(u32 read_addr)
{
	return *(u16*)read_addr;	
}
