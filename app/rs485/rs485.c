#include "rs485.h"
void rs485_init(u32 baund)   //485初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //声明一个结构体变量，用来初始化GPIO

	USART_InitTypeDef USART_InitStructure;	 //串口结构体定义

	NVIC_InitTypeDef NVIC_InitStructure;	//中断结构体定义

	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;	//TX-485	//串口输出PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);				 	/* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//CS-485
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);		   
	GPIO_ResetBits(GPIOG,GPIO_Pin_3);//首先要打开485接收数据的控制端

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;	//RX-485	   //串口输入PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);			 /* 初始化GPIO */


	USART_InitStructure.USART_BaudRate = baund;			   //波特率设置为9600	//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	   //数据长8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		   //无效验
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //失能硬件流
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		 //开启发送和接受模式
	USART_Init(USART2, &USART_InitStructure);	   /* 初始化USART2 */
	USART_Cmd(USART2,ENABLE);	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //使能或者失能指定的USART中断 接收中断
	USART_ClearFlag(USART2,USART_FLAG_TC);	 //清除USARTx的待处理标志位

	/* 设置NVIC参数 */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 	   //打开USART2的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 		 //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		 //使能
	NVIC_Init(&NVIC_InitStructure);		
}
void USART2_IRQHandler(void)	//485通信中断函数
{
	static u8 k;
	USART_ClearFlag(USART2,USART_FLAG_TC);
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//检查指定的USART中断发生与否	
	{
		k=USART_ReceiveData(USART2);
		GPIO_SetBits(GPIOG,GPIO_Pin_3);
		delay_ms(1);
		USART_SendData(USART2,k);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
		delay_ms(2);		
		GPIO_ResetBits(GPIOG,GPIO_Pin_3);	
	}
}
