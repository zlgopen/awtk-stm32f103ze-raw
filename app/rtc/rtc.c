#include "rtc.h"
#include "usart.h"

_calendar_obj calendar;//时钟结构体
u8 rtc_data[7],rtc_bit=0;

void RTC_NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void RTC_IRQHandler()
{
	if(RTC_GetITStatus(RTC_IT_SEC)!= RESET) //进入秒中断
	{
		RTC_Get();  //更新时间
		rtc_bit=1;		
	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	
	  	RTC_Get();				//更新时间   
  		printf("Alarm Time:%d-%d-%d %d:%d:%d\n",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);//输出闹铃时间		
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	
}

//返回值0：初始化正常，1：不正常
u8 rtc_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);//打开后备备份区域时钟和电源时钟
	PWR_BackupAccessCmd(ENABLE); //使能RTC和后备区域寄存器访问
	if(BKP_ReadBackupRegister(BKP_DR1)!=0x5050) //读取后备寄存器中BKP_DR1中的值，如果不是0x5050第一次初始化配置
	{
		BKP_DeInit(); //将BKP内全部寄存器复位
		RCC_LSEConfig(RCC_LSE_ON);   //设置外部低速晶振（ LSE）
		/*while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET&&temp<800)//判断外部32.768K晶振是否准备好
		{
			temp++;	
			delay_ms(10);
		}
		if(temp>=800)	 //时钟检测超时返回一个错误信号
		{
			return 1;
		}*/
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);	  //设置RTC时钟
		RCC_RTCCLKCmd(ENABLE);	 //使能RTC时钟
		RTC_WaitForLastTask(); //等待最近一次对 RTC 寄存器的写操作完成
		RTC_WaitForSynchro();  //等待RTC寄存器时钟同步
		RTC_ITConfig(RTC_IT_SEC,ENABLE); //使能秒中断
		RTC_WaitForLastTask(); //等待最近一次对 RTC 寄存器的写操作完成
		RTC_EnterConfigMode(); //进入 RTC 配置模式
		RTC_SetPrescaler(32767);  //设置预分频值 使用之前一定要进入RTC模式配置，之前还要等待RTC寄存器操作完成
		RTC_WaitForLastTask(); //等待最近一次对 RTC 寄存器的写操作完成
		RTC_Set(2015,10,30,13,59,56);  //设置当前时间
		RTC_ExitConfigMode(); //退出 RTC 配置模式
		BKP_WriteBackupRegister(BKP_DR1,0x5050);  //向指定的后备寄存器中写入用户程序数据
	}
	else  //当不是第一次进入系统那么继续计时,判断标准就是通过后备寄存器BKP_DR1的值0x5050
	{
		RTC_WaitForSynchro();  //等待RTC寄存器时钟同步
		RTC_ITConfig(RTC_IT_SEC,ENABLE); //使能秒中断
		RTC_WaitForLastTask(); //等待最近一次对 RTC 寄存器的写操作完成			
	}
	RTC_NVIC_Config();  //RTC的中断分组设置函数
	RTC_Get();  //RTC更新时间函数
	return 0;		
}


//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是  0,不是
//能被4整除不能被100整除的或者能被400整除的是闰年，其他的是平年
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 		

//以 1970 年 1 月 1 日0时0分0秒为基准
//1970~2099 年为合法年份
//返回值:0,成功;其他:错误代码.
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};//平年月份天数
u8 RTC_Set(u16 year,u8 month,u8 day,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(year<1970||year>2099) //超过年限返回错误
	{
		return 1;
	}
	for(t=1970;t<year;t++)	 //把所有年数的秒数累加
	{
		if(Is_Leap_Year(t))	  //判断该年是否为闰年
		{
			seccount+=31622400;	//闰年的秒数	
		}
		else
		{
			seccount+=31536000;	//平年的秒数	
		}	
	}
	month-=1;	//计算到前面一个月，后面通过天数来计算秒
	for(t=0;t<month;t++)	//把前面月份的秒数累加
	{
		seccount+=(u32)mon_table[t]*86400; //每一天的秒数*天数就是月份秒数
		if(Is_Leap_Year(year)&&t==1)  //如果是闰年的2月份比平年增加一天的秒数
		{
			seccount+=86400; //每一天的秒数*天数就是月份秒数	
		}			
	}
	seccount+=(u32)(day-1)*86400; //把前面日期的秒数累加
	seccount+=(u32)hour*3600; //把时的秒数累加
	seccount+=(u32)min*60; //把分的秒数累加
	seccount+=(u32)sec; //把秒的秒数累加
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE); //使能RTC和后备区域寄存器访问
	RTC_SetCounter(seccount);	//设置RTC计数器的值
	RTC_WaitForLastTask(); //等待最近一次对 RTC 寄存器的写操作完成
	return 0;		
}

//初始化闹钟		  
//以1970年1月1日为基准
//1970~2099年为合法年份
//syear,smon,sday,hour,min,sec：闹钟的年月日时分秒   
//返回值:0,成功;其他:错误代码.
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去 			    
	//设置时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	//上面三步是必须的!
	
	RTC_SetAlarm(seccount);
 
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	    
}


//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}			  


u8 RTC_Get()  //得到当前的时间保存在 calendar 结构体里面  返回0：成功，1：错误
{
	static u16 daycnt=0;
	u32 timecount=0;
	u32 temp=0;
	u16 temp1=0;
	timecount=RTC_GetCounter();	//获取RTC计数器中的值 秒数
	temp=timecount/86400; //得到天数
	if(daycnt!=temp)  //如果天数大于等于1
	{
		daycnt=temp;
		temp1=1970;  //从1970年开始
		while(temp>=365)
		{
			if(Is_Leap_Year(temp1)) //如果是闰年
			{
				if(temp>=366)
				{
					temp-=366;	 //闰年的秒数
				}
				else 
				{
					temp1++;
					break;
				}
						
			}
			else temp-=365;	 //平年的秒数
			temp1++;
		}
		calendar.w_year=temp1;	//得到年份
		temp1=0;
		while(temp>=28)  //超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2 月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break;
			}
			else
			{ 
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;	
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期
	}
	temp=timecount%86400; //得到秒钟数 
	calendar.hour=temp/3600; //小时
	calendar.min=(temp%3600)/60; //分钟
	calendar.sec=(temp%3600)%60; //秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);
	//获取星期
	return 0;	
}
