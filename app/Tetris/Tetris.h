#ifndef __TETRIS__H
#define __TETRIS__H
#include "system.h"

/*************俄罗斯方块游戏参数*****************/
#define BPIXEL 10	//一个小方块边长的像素值
#define	X_BOXS 14	//游戏底板X轴小方块个数
#define Y_BOXS 25	//游戏底板Y轴小方块个数
#define SHAPE_COLOR 0x0000	//图形颜色->黑色
#define BOXS_COLOR	0X07FF	//底板颜色->白色
#define WILL_COLOR	0x7D7C	//边界颜色->浅蓝色
#define TEXT_COLOR	0x0000	//字体颜色->黑色

#define LEFT 	34
#define	RIGHT 	194
#define DOWN	168
#define TURN	98
#define RESET 	162
extern u8 score_buf[7],level_buf[4];
extern u16 BoxSR[Y_BOXS+4];
typedef struct
{
	u16	x;
	u16 y;
	u16 color;
	u8  CurNum;	//当前图形编号
	u8	NextNum;//下一个图形编号
	u8	TurnNum;//翻转图形编号
	u8 	Move;
	
}_Shape;
extern _Shape NewShape;

typedef struct
{
	u16 speed;
	u8  level;
	u32 score;
}_Game;
extern _Game Game;

//void TIM4_Init(u16 arr,u16 psc);	//控制俄罗斯方块自由下落的速度
void Show_TetrisFace(void);	//显示俄罗斯方块游戏主界面
void Show_NextShape(u16 sx,u16 sy,u16 color);	//显示下一个方块图形
void Start_Game(void);	//开始游戏
void LCD_DrawBlock(u16 sx,u16 sy,u16 color);	//LCD画小方块函数
void LCD_DrawShape(u16 sx,u16 sy,u8 n,u16 color);	//LCD画图形函数
void LCD_ClearShape(u16 sx,u16 sy,u8 n,u16 color);//清除图形函数
u8 Judge(u16 sx,u16 sy,u8 n,u8 mode);
void Create_Shape(void);

void MoveLeft(void);
void MoveRight(void);
void DownFast(void);
void Transform(void);
void DownFree(void);
void ResetGame(void);

#endif
