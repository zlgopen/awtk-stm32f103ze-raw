#include "button.h"
#include "delay.h"
#include "flash.h"
#include "gui.h"
#include "led.h"
#include "rtc.h"
#include "stdlib.h"
#include "sys.h"
#include "tim.h"
#include "touch.h"
#include "usart.h"
#include "lcd_driver.h"

extern void systick_init(void);
extern int gui_app_start(int lcd_w, int lcd_h);

int main()
{	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  systick_init();
  led_init();
  button_init();
  TFT_Init();
  TFT_ClearScreen(BLACK);
  FLASH_Init();
  TOUCH_Init();

  TIM3_Init(50, 7199);
  rtc_init();

	return gui_app_start(320, 480);
}
