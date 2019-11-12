#include "ball.h"
#include "tftlcd.h"
#include "key.h"
#include "picture.h"


struct BALL ball={1,100,100};
struct PAI pai={1,100,297};
struct GAME game={1,0,0};
u8 button;
u8 buf[4];


void LCD_ShowPictureEx(u16 x, u16 y, u16 wide, u16 high)
{
	u16 temp = 0;
	long tmp=0,num=0;
	LCD_Set_Window(x, y, x+wide-1, y+high-1);
	num = wide * high*2 ;
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		LCD_WriteData_Color(temp);//逐点显示
		tmp += 2;
	}
	while(tmp < num);	
}

void GUI_DotP(u16 x,u16 y,u16 color)
{
	u8 i,j;
	LCD_Set_Window(x, y, x+29, y+1);	
	for(i=0;i<2;i++)
	{
		for(j=0;j<30;j++)
		{	
			LCD_WriteData_Color(color);		
		}
	}		
}

void GUI_DotB(u16 x,u16 y,u16 color)
{
	u8 i,j;
	LCD_Set_Window(x, y, x+4, y+4);	
	for(i=0;i<5;i++)
	{
		for(j=0;j<5;j++)
		{	
			LCD_WriteData_Color(color);		
		}
	}		
}

void game_init_show(void)	  //游戏初始化显示
{
	ball.dir=1;
	ball.x=100;
	ball.y=100;		   //设置初始方向和坐标
	pai.x=100;
	pai.y=297;	   //设置球拍的初始值
	game.life=1;
	game.score=0;
	LCD_Clear(BLACK);
	FRONT_COLOR=GREEN;
	LCD_DrawRectangle(0,0,239,399);
	LCD_DrawLine(0,300,239,300);
	GUI_DotP(pai.x,pai.y,WHITE);
	LCD_ShowString(10,330,tftlcd_data.width,tftlcd_data.height,16,"Game Socre:");
	
}

void gameover_show(void)  //游戏结束显示
{
	
	LCD_Clear(BLACK);
	LCD_ShowString(90,100,tftlcd_data.width,tftlcd_data.height,16,"Game Over!");
	LCD_ShowString(30,120,tftlcd_data.width,tftlcd_data.height,16,"Press K_UP Key Start...");
	buf[0]=game.score/100+0x30;
	buf[1]=game.score%100/10+0x30;
	buf[2]=game.score%100%10+0x30;
	buf[3]='\0';
	LCD_ShowString(10,150,tftlcd_data.width,tftlcd_data.height,16,"Game Socre:");
	LCD_ShowString(100,150,tftlcd_data.width,tftlcd_data.height,16,buf);
	
	while(1)
	{
		if(KEY_Scan(0)==KEY_UP)
		{
			game_init_show();
			TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
			TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
			TIM_Cmd(TIM3,ENABLE);	 
			TIM_Cmd(TIM4,ENABLE);
			break;
		}		
	}
			
}
void ball_play(void)
{
	if(game.life==0)   //游戏结束标志
	{	
		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
		TIM_Cmd(TIM3,DISABLE);	 
		TIM_Cmd(TIM4,DISABLE);
		gameover_show();		 
	}
	if(game.sco==1)	  //得分标志
	{
		game.sco=0;
		buf[0]=game.score/100+0x30;
		buf[1]=game.score%100/10+0x30;
		buf[2]=game.score%100%10+0x30;
		buf[3]='\0';
		LCD_ShowString(100,330,tftlcd_data.width,tftlcd_data.height,16,buf);
	}	
}




