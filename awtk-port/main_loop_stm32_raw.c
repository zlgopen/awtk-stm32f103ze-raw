/**
 * File:   main_loop_stm32_raw.c
 * Author: AWTK Develop Team
 * Brief:  main loop for stm32
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-02-17 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "key.h"
#include "led.h"
#include "rtc.h"
#include "stdlib.h"
#include "tftlcd.h"
#include "touch.h"
#include "usart.h"


#include "base/idle.h"
#include "base/timer.h"
#include "tkc/platform.h"
#include "base/main_loop.h"
#include "base/event_queue.h"
#include "base/font_manager.h"
#include "lcd/lcd_mem_fragment.h"
#include "main_loop/main_loop_simple.h"


ret_t platform_disaptch_input(main_loop_t *l) { return RET_OK; }

static lcd_t *platform_create_lcd(wh_t w, wh_t h) {
  return lcd_mem_fragment_create(w, h);
}

void dispatch_input_events(void) {
  static int prev_key = 0;
  int key = KEY_Scan(0);

  switch (key) {
  case KEY_RIGHT: {
    key = TK_KEY_RIGHT;
    break;
  }
  case KEY_DOWN: {
    key = TK_KEY_DOWN;
    break;
  }
  case KEY_LEFT: {
    key = TK_KEY_RETURN;
    break;
  }
  case KEY_UP: {
    key = TK_KEY_UP;
    break;
  }
  default: { key = 0; }
  }

  if (key != prev_key) {
    main_loop_post_key_event(main_loop(), FALSE, prev_key);
    main_loop_post_key_event(main_loop(), TRUE, key);
  } else {
    main_loop_post_key_event(main_loop(), TRUE, key);
  }
  prev_key = key;

  if (TOUCH_Scan() == 0) {
    main_loop_post_pointer_event(main_loop(), TRUE, TouchData.lcdx,
                                 TouchData.lcdy);
  } else {
    main_loop_post_pointer_event(main_loop(), FALSE, TouchData.lcdx,
                                 TouchData.lcdy);
  }
}

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    dispatch_input_events();
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
  }
}

#include "main_loop/main_loop_raw.inc"
