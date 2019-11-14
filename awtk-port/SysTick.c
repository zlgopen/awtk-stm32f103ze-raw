#include "SysTick.h"

static u8 fac_us = 0;  // us延时倍乘数
static u16 fac_ms = 0; // ms延时倍乘数

void systick_enable_int(void) {
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  SysTick->LOAD = fac_ms;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

//初始化延迟函数
// SYSTICK的时钟固定为AHB时钟的1/8
// SYSCLK:系统时钟频率
void SysTick_Init(void) {
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = SystemCoreClock / 8000000;
  fac_ms = (u16)fac_us * 1000;
}

//延时nus
// nus为要延时的us数.
void delay_us(u32 nus) {
  u32 temp = 0;
  SysTick->LOAD = nus * fac_us; //时间加载
  SysTick->VAL = 0x00;          //清空计数器
  do {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
}

//延时nms
//注意nms的范围
// SysTick->LOAD为24位寄存器,所以,最大延时为:
// nms<=0xffffff*8*1000/SYSCLK
// SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(u16 nms) {
  u32 temp = 0;
  SysTick->LOAD = (u32)nms * fac_ms; //时间加载(SysTick->LOAD为24bit)
  SysTick->VAL = 0x00;               //清空计数器
  do {
    temp = SysTick->CTRL;
  } while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
}
