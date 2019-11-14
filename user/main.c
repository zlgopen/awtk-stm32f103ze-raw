#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "time.h"
#include "key.h"
#include "touch.h"
#include "rtc.h"
#include "stdlib.h"

extern int gui_app_start(int lcd_w, int lcd_h);
extern void systick_enable_int(void);

void hardware_prepare(void) {
	SysTick_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			
	KEY_Init();
	TOUCH_Init();
	TIM3_Init(50,7199);
	RTC_Init();
}

int main() {
	hardware_prepare();
	systick_enable_int();
	
	return gui_app_start(tftlcd_data.width, tftlcd_data.height);
}
