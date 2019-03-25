/**
 * File:   lcd_stm32_raw.c
 * Author: AWTK Develop Team
 * Brief:  stm32_raw implemented lcd interface
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "gui.h"
#include "lcd_driver.h"

#include "tkc/mem.h"
#include "lcd/lcd_reg.h"

typedef uint16_t pixel_t;

#define LCD_FORMAT BITMAP_FMT_BGR565
#define pixel_from_rgb(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define pixel_to_rgba(p) {(0xff & ((p >> 11) << 3)), (0xff & ((p >> 5) << 2)), (0xff & (p << 3))}

#define set_window_func TFT_SetWindow
#define write_data_func TFT_WriteData

#include "base/pixel.h"
#include "blend/pixel_ops.inc"
#include "lcd/lcd_reg.inc"
