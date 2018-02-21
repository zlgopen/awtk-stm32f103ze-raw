#ifndef __Tetris_H__
#define __Tetris_H__
#include "sys.h"


#define RightB		0XFE
#define LeftB		0XFD
#define UpB			0XF7
#define DownB		0XFB
#define AB			0X7F
#define BB			0XBF
#define SelectB	0XDF
#define StartB		0XEF
extern u16 POINT_COLOR;

extern u8 s;
extern u8 Name[6];
void Draw_Block(u16 x,u16 y,u16 a,u16 b);
void Draw_MainMenu(void);
void Draw_ARROW(u8 s,u16 color);
void Get_Select(void);
void Display_Help(void);
void Display_Rank(void);
void Write_Name(void);
void Clear_Rank(void);
void Draw_Screen(void);
void Draw_Block_1(u16 x,u16 y,u8 m,u8 s,u8 c);
void Draw_Block_2(u16 x,u16 y,u8 s,u8 c);
void Draw_Block_3(u16 x,u16 y,u8 m,u8 s,u8 c);
void Draw_Block_4(u16 x,u16 y,u8 m,u8 s,u8 c);
void Draw_Block_5(u16 x,u16 y,u8 m,u8 s,u8 c);
void Draw_Block_6(u16 x,u16 y,u8 m,u8 s,u8 c);
void Draw_Block_7(u16 x,u16 y,u8 m,u8 s,u8 c);
void Show_Block(u16 x,u16 y,u8 k,u8 s,u8 m,u8 c);
void Get_Position(u16 x,u16 y,u8 k,u8 m);
void Game_Over(void);
void Show_Scores(void);
u16 Get_Level_Scores(void);
void Check_Scores(void);
void Show_Level(void);

#endif
