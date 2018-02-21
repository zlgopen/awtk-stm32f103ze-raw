#ifndef _rtc_H
#define _rtc_H
#include "sys.h"
#include "delay.h"

//时间结构体
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	//公历日月年周
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体
extern u8 rtc_data[7],rtc_bit;
extern u8 const mon_table[12];	//月份日期数据表

u8 rtc_init(void);        //初始化RTC,返回0,失败;1,成功;
u8 Is_Leap_Year(u16 year);//平年,闰年判断
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //更新时间   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec);//设置时间
#endif
