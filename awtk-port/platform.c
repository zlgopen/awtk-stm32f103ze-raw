/**
 * file:   platform.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  platform dependent function of stm32
 *
 * copyright (c) 2018 - 2018 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "delay.h"
#include "base/mem.h"
#include "base/timer.h"

uint32_t get_time_ms() {
  uint32_t cnt = 0;
  /*TODO*/
  return cnt;
}

void sleep_ms(uint32_t ms) { delay_ms(ms); }

static uint32_t s_heam_mem[2048];

ret_t platform_prepare(void) {
  timer_init(get_time_ms);
  mem_init(s_heam_mem, sizeof(s_heam_mem));

  return RET_OK;
}
