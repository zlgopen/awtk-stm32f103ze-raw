#ifndef _sram_H
#define _sram_H

#include "sys.h"
#include "delay.h"
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuf(u8 *pbuf,u32 bufaddr,u32 len);
void FSMC_SRAM_ReadBuf(u8 *pbuf,u32 readaddr,u32 len);
void FSMC_SRAM_Test_Write(u8 dat,u32 wirte_addr);
u8 FSMC_SRAM_Test_Read(u32 read_addr);


#define SRAM_ADDR ((u32)0x68000000)	 //FSMC_Bank1-3ÆðÊ¼µØÖ·

#endif
