#ifndef _DS18B20_H
#define _DS18B20_H
#include "sys.h"
#include "delay.h"

#define dq (GPIO_Pin_11) //PG11
#define GPIO_ds18b20 GPIOG
#define ds18b20_dq_H GPIO_SetBits(GPIO_ds18b20,dq)
#define ds18b20_dq_L GPIO_ResetBits(GPIO_ds18b20,dq)
#define DS18B20_DQ_IN PGin(11)
u8 ds18b20_init(void);
u8 ds18b20_Check(void);
void DQOUTINT(void);	 // ‰≥ˆ≈‰÷√
void DQININT(void);	 // ‰»Î≈‰÷√
void ds18b20init(void);
void ds18b20wr(u8 dat);
u8 DS18b20rd(void);
double readtemp(void);
void ds18b20rst(void);


#endif
