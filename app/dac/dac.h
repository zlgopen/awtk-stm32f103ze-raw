#ifndef _dac_H
#define _dac_H

#include "sys.h"
#include "delay.h"
void dac1_init(void);
void Set_Channel_val(u16 dat);  //设置DAC通道数据 0-3300（0-3.3V）


#endif
