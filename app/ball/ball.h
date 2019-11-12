#ifndef _ball_H
#define _ball_H
#include "system.h"
#include "SysTick.h"


struct BALL
{
	u8 dir;  //方向		1:右上角 2：左上角 3：左下角 4：右下角
	u8 x;  //x坐标
	u16 y;  //y坐标
};
extern struct BALL ball;

struct PAI
{
	u8 dir;	  //球拍X坐标
	int x;	  //球拍Y坐标
	u16 y;	  //球拍方向  1：左 2：右 3：静止
};
extern struct PAI pai;

struct GAME
{
	u8 life;	  //生命
	u16 score;	  //分数
	u8 sco;   //得分标志
};
extern struct GAME game;
extern u8 button;
void GUI_DotP(u16 x,u16 y,u16 color);
void GUI_DotB(u16 x,u16 y,u16 color);
void game_init_show(void);	  //游戏初始化显示
void ball_play(void);	  //球拍显示
void LCD_ShowPictureEx(u16 x, u16 y, u16 wide, u16 high);
#endif
