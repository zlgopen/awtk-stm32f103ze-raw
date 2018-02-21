#include "Tetris.h"
#include "delay.h"
#include "lcd_driver.h"
#include "gui.h"
#include "button.h"
#include "AT24CXX.h"
#include "Tfont.h"
#include "led.h"
#include "tim.h"

u16 POINT_COLOR=0x0000;

void Draw_Block(u16 x,u16 y,u16 a,u16 b)
{

	u8 w,h;
	for(h=0;h<a;h++)
	{
		LCD_SetCursor(x,h+y);
		TFT_WriteCmd(0x0202);
		for(w=0;w<b;w++) 
		{
			TFT_WriteData(POINT_COLOR);	
		}
	} 
}

void Draw_MainMenu()
{
	u8 temp,t1,t2;
	u16 x=24,y=32,t;
	u16 y0=y;
	for(t2=0;t2<6;t2++)
	{
		for(t=0;t<256;t++)
		{  
			temp=TETRIS_FONT[t2][t];
			for(t1=0;t1<8;t1++)
			{	
				if(temp&0x80)GUI_Dot(x,y,RED);
				else GUI_Dot(x,y,WHITE);	
				temp<<=1;
				y++;
				if((y-y0)==64)
				{
					y=y0;
					x++;
				}
			}  	 
		 } 
	 }
	 x=88;
	 y=112;
	 y0=y;
	 for(t2=0;t2<4;t2++)
	{
		for(t=0;t<64;t++)
		{  
			temp=PLAY[t2][t];
			for(t1=0;t1<8;t1++)
			{	
				if(temp&0x80)GUI_Dot(x,y,BLUE);
				else GUI_Dot(x,y,WHITE);	
				temp<<=1;
				y++;
				if((y-y0)==32)
				{
					y=y0;
					x++;
				}
			}  	 
		 } 
	 }
	 x=88;
	 y=160;
	 y0=y;
	 for(t2=0;t2<4;t2++)
	{
		for(t=0;t<64;t++)
		{  
			temp=RANK[t2][t];
			for(t1=0;t1<8;t1++)
			{	
				if(temp&0x80)GUI_Dot(x,y,BLUE);
				else GUI_Dot(x,y,WHITE);	
				temp<<=1;
				y++;
				if((y-y0)==32)
				{
					y=y0;
					x++;
				}
			}  	 
		 } 
	 }
	 x=88;
	 y=208;
	 y0=y;
	 for(t2=0;t2<4;t2++)
	{
		for(t=0;t<64;t++)
		{  
			temp=HELP[t2][t];
			for(t1=0;t1<8;t1++)
			{	
				if(temp&0x80)GUI_Dot(x,y,BLUE);
				else GUI_Dot(x,y,WHITE);	
				temp<<=1;
				y++;
				if((y-y0)==32)
				{
					y=y0;
					x++;
				}
			}  	 
		 } 
	 }
	 GUI_Show12ASCII(24,288,"Press A Button to Enter",BROWN,WHITE);
 }
 
 void Draw_ARROW(u8 s,u16 color)
 {
	 u8 temp,t1,t2;
	 u16 x,y=(s-1)*48+112,t;
	 u16 y0=y;
	 x=40;
	 for(t2=0;t2<3;t2++)
	 {
		for(t=0;t<64;t++)
		{  
			temp=ARROW_L[t2][t];
			for(t1=0;t1<8;t1++)
			{	
				if(temp&0x80)GUI_Dot(x,y,color);
				else GUI_Dot(x,y,WHITE);	
				temp<<=1;
				y++;
				if((y-y0)==32)
				{
					y=y0;
					x++;
				}
			}  	 
		 } 
	 }
	 x=152;
	 y=(s-1)*48+112;
	 for(t2=0;t2<3;t2++)
	 {
		for(t=0;t<64;t++)
		{  
			temp=ARROW_R[t2][t];
			for(t1=0;t1<8;t1++)
			{	
				if(temp&0x80)GUI_Dot(x,y,color);
				else GUI_Dot(x,y,WHITE);	
				temp<<=1;
				y++;
				if((y-y0)==32)
				{
					y=y0;
					x++;
				}
			}  	 
		 } 
	 }
}

u8 s=1;
void Get_Select()
{
	u8 t;
	t=keyscan(0);
	if(t==K_UP_PRESS)
	{	
		Draw_ARROW(s,WHITE);
		s--;
		if(s<1) s=3;
		Draw_ARROW(s,BLACK);
	}
	else if(t==K_DOWN_PRESS)
	{
		Draw_ARROW(s,WHITE);
		s++;
		if(s>3) s=1;
		Draw_ARROW(s,BLACK);	
	}
}

extern u8 h_menu;
void Display_Help()
{
	u8 i;
	TFT_ClearScreen(WHITE);

	GUI_Show12ASCII(24,32,"How To Play:",RED,WHITE);
	GUI_Show12ASCII(24,64,"   Press Left Button to move the block to left,and press Right Buttom to move the block to right.Press B Button to let the block get down more quickly,and Press A Button to change the block.Just Fill the Blank to kill the bottom block and get Scores.When the Score is enough,you will get into next Level!",RED,WHITE);
	GUI_Show12ASCII(24,288,"Press B Button to Return",BLUE,WHITE);
	while(keyscan(0) != K_RIGHT_PRESS)
	{
		i++;
		if(i==20)
		{
			i=0;
			led1=~led1;
		}
		delay_ms(10);
	}
   if(h_menu==1)
   {
      TFT_ClearScreen(WHITE);
      Draw_MainMenu();
      Draw_ARROW(s,BLACK);
   }
}

u8 Name[6];
extern u16 Scores;
/*void Display_Rank()
{
	u8 t;
	u16 h,i;
	u16 scor;
	u8 scor_buf[6];
	TFT_ClearScreen(WHITE);
	GUI_Show12ASCII(68,32,"Top 10 Player",RED,WHITE);
	h=64;

	for(t=0;t<10;t++)
	{
		AT24CXX_Read(8+t*8,Name,6);
		scor=AT24CXX_ReadLenData(14+t*8);
		GUI_Show12ASCII(56,h,Name,BLUE,WHITE);
		scor_buf[0]=scor/10000+0x30;
		scor_buf[1]=scor%10000/1000+0x30;
		scor_buf[2]=scor%10000%1000/100+0x30;
		scor_buf[3]=scor%10000%1000%100/10+0x30;
		scor_buf[4]=scor%10000%1000%100%10+0x30;
		scor_buf[5]='\0';
		GUI_Show12ASCII(56+11*8,h,scor_buf,BLUE,WHITE);
		h+=24;
	}
	GUI_Show12ASCII(40,304,"B: Return   A: Clear",BROWN,WHITE);
	delay_ms(200);
	while(keyscan(0) != K_RIGHT_PRESS && keyscan(0) != K_LEFT_PRESS)
	{
		i++;
		if(i==20)
		{
			i=0;
			led1=~led1;
		}
		delay_ms(10);
	}
	if(keyscan(0) == K_RIGHT_PRESS)
	{
      if(h_menu==1)
      {
		 TFT_ClearScreen(WHITE);
	      Draw_MainMenu();
	      Draw_ARROW(s,BLACK);
      }
	}
	else if(keyscan() == K_LEFT_PRESS)
	{
		Clear_Rank();
	}
} 

void Write_Name()
{
	u8 t,h,i=20,j,n=65;
	u8 Name1[6];
	u16 temp,temp1=Scores;
	GUI_Box(40,130,199,189,GREEN);
	GUI_Show12ASCII(60,130,"Congratulation!",RED,GREEN);
	GUI_Show12ASCII(56,150,"Write Your Name!",RED,GREEN);
	GUI_Show12ASCII(76,170,"_ _ _ _ _ _ ",RED,GREEN);
	delay_ms(300);
	for(t=0;t<6;t++)
	{
		n=65;
		Name[t]=n;
		GUI_Show12ASCII(76+t*16,170,n,16,1);
		while(Joypad_Read() != AB)
		{
			if(i==40)
			{
				i=0;
				LCD_ShowChar(84+t*16,170,'|',16,1);
				LED0=~LED0;
			}
			i++;
			delay_ms(5);
			if(j==40)
			{
				j=0;
				POINT_COLOR=GREEN;
				LCD_ShowChar(84+t*16,170,'|',16,1);
				POINT_COLOR=RED;
				LED1=~LED1;
			}
			j++;
			delay_ms(5);
			if(Joypad_Read()==DownB)
			{
				delay_ms(5);
				if(Joypad_Read()==DownB)
				{
					n++;
					if(n>90) n=65;
					LCD_Fill(76+t*16,170,84+t*16,184,GREEN);
					LCD_ShowChar(76+t*16,170,n,16,1);
					Name[t]=n;
					while(Joypad_Read() != 0xFF);
				}
			}
			if(Joypad_Read()==UpB)
			{
				delay_ms(5);
				if(Joypad_Read()==UpB)
				{
					n--;
					if(n<65) n=90;
					LCD_Fill(76+t*16,170,84+t*16,184,GREEN);
					LCD_ShowChar(76+t*16,170,n,16,1);
					Name[t]=n;
					while(Joypad_Read() != 0xFF);
				}
			}
		}
		POINT_COLOR=GREEN;
		LCD_ShowChar(84+t*16,170,'|',16,1);
		POINT_COLOR=RED;
		delay_ms(300);
	}
	for(t=0;t<10;t++)
	{
		temp=AT24CXX_ReadLenData(14+8*t);
		AT24CXX_ReadData(8+8*t,Name1,6);
		if(temp1>=temp)
		{
			AT24CXX_WriteData(8+8*t,Name,6);
			AT24CXX_WriteLenData(14+8*t,temp1);
			for(h=t+1;h<10;h++)
			{
				temp1=AT24CXX_ReadLenData(14+8*h);
				AT24CXX_ReadData(8+8*h,Name,6);
				AT24CXX_WriteData(8+8*h,Name1,6);
				AT24CXX_WriteLenData(14+8*h,temp);
				temp=temp1;
				for(i=0;i<6;i++) Name1[i]=Name[i];
			}
			break;
		}
		
	}
 	Display_Rank();
}

void Clear_Rank()
{
	u8 t;
	LCD_Fill(20,140,220,180,GREEN);
	POINT_COLOR=RED;
	BACK_COLOR=GREEN;
	LCD_ShowString(44,140,200,16,16,"Sure to clear Rank?");
	LCD_ShowString(44,160,200,16,16,"  A: Yes    B: No  ");
	delay_ms(100);
	while(1)
	{
		if(Joypad_Read()==AB)
		{
			delay_ms(5);
			if(Joypad_Read()==AB)
			{
				while(Joypad_Read() != 0xFF);
				AT24CXX_ClearData(0,96);
				break;
			}
		}
		else if(Joypad_Read()==BB)
		{
			delay_ms(5);
			if(Joypad_Read()==BB)
			{
				break;
			}
		}
		t++;
		if(t%20==0) LED1=~LED1;
		delay_ms(10);
	}
	Display_Rank();
} */

extern u16 Level,Level_Up;
void Draw_Screen()
{
	u8 level_buf[3],levelup_buf[6],score_buf[6];
	level_buf[0]=Level/10+0x30;
	level_buf[1]=Level%10+0x30;
	level_buf[2]='\0';

	levelup_buf[0]=Level_Up/10000+0x30;
	levelup_buf[1]=Level_Up%10000/1000+0x30;
	levelup_buf[2]=Level_Up%10000%1000/100+0x30;
	levelup_buf[3]=Level_Up%10000%1000%100/10+0x30;
	levelup_buf[4]=Level_Up%10000%1000%100%10+0x30;
	levelup_buf[5]='\0';

	score_buf[0]=Scores/10000+0x30;
	score_buf[1]=Scores%10000/1000+0x30;
	score_buf[2]=Scores%10000%1000/100+0x30;
	score_buf[3]=Scores%10000%1000%100/10+0x30;
	score_buf[4]=Scores%10000%1000%100%10+0x30;
	score_buf[5]='\0';

	GUI_Line(160,0,161,320,BLACK);
	GUI_Line(161,0,162,320,BLACK);
	GUI_Show12ASCII(181,24,"LEVEL",RED,WHITE);
	GUI_Show12ASCII(193,48,level_buf,RED,WHITE);
	LCD_DrawRectangle(177,80,225,128,GRAYBLUE);

	GUI_Show12ASCII(169,152,"LEVEL UP",BROWN,WHITE);
	GUI_Show12ASCII(181,184,levelup_buf,BROWN,WHITE);
	GUI_Show12ASCII(177,224,"Scores",BLUE,WHITE);
	GUI_Show12ASCII(181,256,score_buf,BLUE,WHITE);
	GUI_Show12ASCII(169,288,"Sta:Menu",BLACK,WHITE);
	GUI_Show12ASCII(169,304,"Sel:Paus",BLACK,WHITE);
}

extern u16 xc[4],yc[4];
void Draw_Block_1(u16 x,u16 y,u8 m,u8 s,u8 c)     //直条 m=0:竖直;m=1:横着
{
	u8 i;
	if(c==0) POINT_COLOR=RED;
	else POINT_COLOR=WHITE;
	if(s==0)
	{
		if(m==0)
		{
			Draw_Block(x,y,64,16);
			for(i=0;i<4;i++) 
			{
				xc[i]=x+8;
				yc[i]=y+i*16+8;
			}
		}
		if(m==1)
		{
			Draw_Block(x-16,y+16,16,64);
			for(i=0;i<4;i++)
			{
				xc[i]=x-16+16*i+8;
				yc[i]=y+16+8;
			}
		}
	}
	else if(s==1)
	{
		Draw_Block(201-4,88,32,8);
	}
}

void Draw_Block_2(u16 x,u16 y,u8 s,u8 c)    //方块
{
   u8 i;
	if(c==0) POINT_COLOR=BLUE;
	else POINT_COLOR=WHITE; 
	if(s==0)
	{
		Draw_Block(x,y,32,32);
      for(i=0;i<2;i++)
      {
         xc[i]=x+8;
         yc[i]=y+8+16*i;
      }
      for(i=2;i<4;i++)
      {
         xc[i]=x+8+16;
         yc[i]=y+8+16*(i-2);
      }
	}
	else
	{
		Draw_Block(193,96,16,16);
	}
}

void Draw_Block_3(u16 x,u16 y,u8 m,u8 s,u8 c)  //土形 m=0: 突起在上;m=1: 突起在右;m=2: 突起在下;m=3: 突起在左
{
	if(c==0) POINT_COLOR=GREEN;
	else POINT_COLOR=WHITE;
	if(s==0)
	{
		if(m==0)
		{
			Draw_Block(x,y,16,16);
			Draw_Block(x-16,y+16,16,48);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x-16+8;
         yc[2]=y+16+8;
         xc[3]=x+16+8;
         yc[3]=y+16+8;
		}
		if(m==1)
		{
			Draw_Block(x,y,48,16);
			Draw_Block(x+16,y+16,16,16);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x+8;
         yc[2]=y+8+32;
         xc[3]=x+16+8;
         yc[3]=y+16+8;
		}
		if(m==2)
		{
			Draw_Block(x-16,y+16,16,48);
			Draw_Block(x,y+32,16,16);
         xc[0]=x+8;
         yc[0]=y+8+16;
         xc[1]=x+8;
         yc[1]=y+8+32;
         xc[2]=x-16+8;
         yc[2]=y+16+8;
         xc[3]=x+16+8;
         yc[3]=y+16+8;
		}
		else if(m==3)
		{
			Draw_Block(x,y,48,16);
			Draw_Block(x-16,y+16,16,16);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x+8;
         yc[2]=y+32+8;
         xc[3]=x-16+8;
         yc[3]=y+16+8;
		}
	}
	else
	{
		Draw_Block(201-4,96,8,8);
		Draw_Block(193-4,104,8,24);
	}
}

void Draw_Block_4(u16 x,u16 y,u8 m,u8 s,u8 c)  //左Z形 m=0: 左上右下;m=1: 右上左下
{
	if(c==0) POINT_COLOR=YELLOW;
	else POINT_COLOR=WHITE;
	if(s==0)
	{
		if(m==0)
		{
			Draw_Block(x,y,16,32);
			Draw_Block(x+16,y+16,16,32);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8+16;
         yc[1]=y+8;
         xc[2]=x+16+8;
         yc[2]=y+16+8;
         xc[3]=x+32+8;
         yc[3]=y+16+8;
		}
		if(m==1)
		{
			Draw_Block(x,y,32,16);
			Draw_Block(x+16,y-16,32,16);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x+16+8;
         yc[2]=y-16+8;
         xc[3]=x+16+8;
         yc[3]=y+8;
		}
	}
	else
	{
		Draw_Block(193-4,96,8,16);
		Draw_Block(201-4,104,8,16);
	}
}

void Draw_Block_5(u16 x,u16 y,u8 m,u8 s,u8 c)  //右Z形 m=0: 右上左下;m=1: 左上右下
{
	if(c==0) POINT_COLOR=YELLOW;
	else POINT_COLOR=WHITE;
	if(s==0)
	{
		if(m==0)
		{
			Draw_Block(x,y,16,32);
			Draw_Block(x-16,y+16,16,32);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x-16+8;
         yc[2]=y+16+8;
         xc[3]=x+8+16;
         yc[3]=y+8;
		}
		if(m==1)
		{
			Draw_Block(x-16,y,32,16);
			Draw_Block(x,y+16,32,16);
         xc[0]=x+8-16;
         yc[0]=y+8;
         xc[1]=x+8-16;
         yc[1]=y+8+16;
         xc[2]=x+8;
         yc[2]=y+16+8;
         xc[3]=x+8;
         yc[3]=y+32+8;
		}
	}
	else
	{
		Draw_Block(185+4,104,8,16);
		Draw_Block(193+4,96,8,16);
	}
}

void Draw_Block_6(u16 x,u16 y,u8 m,u8 s,u8 c)  //左L形  m=0: 突起在左;m=1: 突起在上;m=2: 突起在右;m=3: 突起在下
{
	if(c==0) POINT_COLOR=BRRED;
	else POINT_COLOR=WHITE;
	if(s==0)
	{
		if(m==0)
		{
			Draw_Block(x,y,48,16);
			Draw_Block(x-16,y+32,16,16);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x+8;
         yc[2]=y+8+32;
         xc[3]=x+8-16;
         yc[3]=y+8+32;
		}
		if(m==1)
		{
			Draw_Block(x-16,y+16,16,16);
			Draw_Block(x-16,y+32,16,48);
         xc[0]=x+8-16;
         yc[0]=y+8+16;
         xc[1]=x+8-16;
         yc[1]=y+8+32;
         xc[2]=x+8;
         yc[2]=y+8+32;
         xc[3]=x+8+16;
         yc[3]=y+8+32;
		}
		if(m==2)
		{
			Draw_Block(x,y,48,16);
			Draw_Block(x+16,y,16,16);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x+8;
         yc[2]=y+8+32;
         xc[3]=x+8+16;
         yc[3]=y+8;
		}
		if(m==3)
		{
			Draw_Block(x-16,y+16,16,48);
			Draw_Block(x+16,y+32,16,16);
         xc[0]=x+8-16;
         yc[0]=y+8+16;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x+8+16;
         yc[2]=y+8+16;
         xc[3]=x+8+16;
         yc[3]=y+8+32;
		}
	}
	else
	{
		Draw_Block(193,104+4,8,8);
		Draw_Block(201,88+4,24,8);
	}
}

void Draw_Block_7(u16 x,u16 y,u8 m,u8 s,u8 c)  //右L形 m=0: 突起在右;m=1: 突起在上;m=2: 突起在左;m=3: 突起在下
{
	if(c==0) POINT_COLOR=BRRED;
	else POINT_COLOR=WHITE;
	if(s==0)
	{
		if(m==0)
		{
			Draw_Block(x,y,48,16);
			Draw_Block(x+16,y+32,16,16);
         xc[0]=x+8;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8+16;
         xc[2]=x+8;
         yc[2]=y+8+32;
         xc[3]=x+8+16;
         yc[3]=y+8+32;
		}
		if(m==1)
		{
			Draw_Block(x-16,y+32,16,48);
			Draw_Block(x+16,y+16,16,16);
         xc[0]=x+8+16;
         yc[0]=y+8+16;
         xc[1]=x+8-16;
         yc[1]=y+8+32;
         xc[2]=x+8;
         yc[2]=y+8+32;
         xc[3]=x+8+16;
         yc[3]=y+8+32;
		}
		if(m==2)
		{
			Draw_Block(x-16,y,16,16);
			Draw_Block(x,y,48,16);
         xc[0]=x+8-16;
         yc[0]=y+8;
         xc[1]=x+8;
         yc[1]=y+8;
         xc[2]=x+8;
         yc[2]=y+8+16;
         xc[3]=x+8;
         yc[3]=y+8+32;
		}
		if(m==3)
		{
			Draw_Block(x-16,y+16,16,48);
			Draw_Block(x-16,y+32,16,16);
         xc[0]=x+8-16;
         yc[0]=y+8+16;
         xc[1]=x+8-16;
         yc[1]=y+8+32;
         xc[2]=x+8;
         yc[2]=y+8+16;
         xc[3]=x+8+16;
         yc[3]=y+8+16;
		}
	}
	else
	{
		Draw_Block(193,88+4,24,8);
		Draw_Block(201,104+4,8,8);
	}
}

extern u8 maxm;
void Show_Block(u16 x,u16 y,u8 k,u8 s,u8 m,u8 c)
{
	switch(k)
	{
		case 1:Draw_Block_1(x,y,m,s,c);maxm=1;break;
		case 2:Draw_Block_2(x,y,s,c);maxm=0;break;
		case 3:Draw_Block_3(x,y,m,s,c);maxm=3;break;
		case 4:Draw_Block_4(x,y,m,s,c);maxm=1;break;
		case 5:Draw_Block_5(x,y,m,s,c);maxm=1;break;
		case 6:Draw_Block_6(x,y,m,s,c);maxm=3;break;
		case 7:Draw_Block_7(x,y,m,s,c);maxm=3;break;
	}
}

void Get_Position(u16 x,u16 y,u8 k,u8 m)
{
   u8 i;
   switch(k)
   {
      case 1:
         if(m==0)
         {
            for(i=0;i<4;i++) 
            {
               xc[i]=x+8;
               yc[i]=y+i*16+8;
            }
         }
         else
         {
            for(i=0;i<4;i++)
            {
               xc[i]=x-16+16*i+8;
               yc[i]=y+16+8;
            }
         }
         break;
      case 2:
         for(i=0;i<2;i++)
         {
            xc[i]=x+8;
            yc[i]=y+8+16*i;
         }
         for(i=2;i<4;i++)
         {
            xc[i]=x+8+16;
            yc[i]=y+8+16*(i-2);
         }
         break;
      case 3:
         if(m==0)
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+8;
            yc[2]=y+8+32;
            xc[3]=x+16+8;
            yc[3]=y+16+8;
         }
         else if(m==1)
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+8;
            yc[2]=y+8+32;
            xc[3]=x+16+8;
            yc[3]=y+16+8;
         }
         else if(m==2)
         {
            xc[0]=x+8;
            yc[0]=y+8+16;
            xc[1]=x+8;
            yc[1]=y+8+32;
            xc[2]=x-16+8;
            yc[2]=y+16+8;
            xc[3]=x+16+8;
            yc[3]=y+16+8;
         }
         else
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+8;
            yc[2]=y+32+8;
            xc[3]=x-16+8;
            yc[3]=y+16+8;
         }
         break;
      case 4:
         if(m==0)
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8+16;
            yc[1]=y+8;
            xc[2]=x+16+8;
            yc[2]=y+16+8;
            xc[3]=x+32+8;
            yc[3]=y+16+8;
         }
         else
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+16+8;
            yc[2]=y-16+8;
            xc[3]=x+16+8;
            yc[3]=y+8;
         }
         break;
      case 5:
         if(m==0)
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x-16+8;
            yc[2]=y+16+8;
            xc[3]=x+8+16;
            yc[3]=y+8;
         }
         else
         {
            xc[0]=x+8-16;
            yc[0]=y+8;
            xc[1]=x+8-16;
            yc[1]=y+8+16;
            xc[2]=x+8;
            yc[2]=y+16+8;
            xc[3]=x+8;
            yc[3]=y+32+8;
         }
         break;
      case 6:
         if(m==0)
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+8;
            yc[2]=y+8+32;
            xc[3]=x+8-16;
            yc[3]=y+8+32;
         }
         else if(m==1)
         {
            xc[0]=x+8-16;
            yc[0]=y+8+16;
            xc[1]=x+8-16;
            yc[1]=y+8+32;
            xc[2]=x+8;
            yc[2]=y+8+32;
            xc[3]=x+8+16;
            yc[3]=y+8+32;
         }
         else if(m==2)
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+8;
            yc[2]=y+8+32;
            xc[3]=x+8+16;
            yc[3]=y+8;
         }
         else
         {
            xc[0]=x+8-16;
            yc[0]=y+8+16;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+8+16;
            yc[2]=y+8+16;
            xc[3]=x+8+16;
            yc[3]=y+8+32;
         }
         break;
      case 7:
         if(m==0)
         {
            xc[0]=x+8;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8+16;
            xc[2]=x+8;
            yc[2]=y+8+32;
            xc[3]=x+8+16;
            yc[3]=y+8+32;
         }
         else if(m==1)
         {
            xc[0]=x+8+16;
            yc[0]=y+8+16;
            xc[1]=x+8-16;
            yc[1]=y+8+32;
            xc[2]=x+8;
            yc[2]=y+8+32;
            xc[3]=x+8+16;
            yc[3]=y+8+32;
         }
         else if(m==2)
         {
            xc[0]=x+8-16;
            yc[0]=y+8;
            xc[1]=x+8;
            yc[1]=y+8;
            xc[2]=x+8;
            yc[2]=y+8+16;
            xc[3]=x+8;
            yc[3]=y+8+32;
         }
         else
         {
            xc[0]=x+8-16;
            yc[0]=y+8+16;
            xc[1]=x+8-16;
            yc[1]=y+8+32;
            xc[2]=x+8;
            yc[2]=y+8+16;
            xc[3]=x+8+16;
            yc[3]=y+8+16;
         }
         break;
   }
}

extern u8 status;
extern u16 Vel;
void Game_Over(void)
{
   GUI_Box(80,150,159,169,RED);
   GUI_Show12ASCII(84,154,"GAME OVER",YELLOW,RED);
 //  BACK_COLOR=WHITE;
   delay_ms(1000);
   delay_ms(1000);
 /*  if(Scores>=AT24CXX_ReadLenData(86))
   {
      h_menu=1;
 //     Write_Name();
   }*/
   status=1;
}

void Show_Scores(void)
{

 	u8 level_buf[3],levelup_buf[6],score_buf[6];
	level_buf[0]=Level/10+0x30;
	level_buf[1]=Level%10+0x30;
	level_buf[2]='\0';

	levelup_buf[0]=Level_Up/10000+0x30;
	levelup_buf[1]=Level_Up%10000/1000+0x30;
	levelup_buf[2]=Level_Up%10000%1000/100+0x30;
	levelup_buf[3]=Level_Up%10000%1000%100/10+0x30;
	levelup_buf[4]=Level_Up%10000%1000%100%10+0x30;
	levelup_buf[5]='\0';

	score_buf[0]=Scores/10000+0x30;
	score_buf[1]=Scores%10000/1000+0x30;
	score_buf[2]=Scores%10000%1000/100+0x30;
	score_buf[3]=Scores%10000%1000%100/10+0x30;
	score_buf[4]=Scores%10000%1000%100%10+0x30;
	score_buf[5]='\0';

	GUI_Show12ASCII(193,48,level_buf,RED,WHITE);

	GUI_Show12ASCII(181,184,levelup_buf,BROWN,WHITE);

	GUI_Show12ASCII(181,256,score_buf,BLUE,WHITE);
}

u16 Get_Level_Scores(void)
{
   u16 scora;
   switch(Level)
   {
      case 1:
         scora=500;
         Vel=4999;
         break;
      case 2:
         scora=1000;
         Vel=4499;
         break;
      case 3:
         scora=1500;
         Vel=3999;
         break;
      case 4:
         scora=2000;
         Vel=3499;
         break;
      case 5:
         scora=3000;
         Vel=2999;
         break;
      case 6:
         scora=4000;
         Vel=2499;
         break;
      case 7:
         scora=5000;
         Vel=1999;
         break;
      case 8:
         scora=7000;
         Vel=1499;
         break;
      case 9:
         scora=9000;
         Vel=999;
         break;
      case 10:
         scora=12000;
         Vel=499;
         break;
   }
   return scora;
}

void Check_Scores(void)
{
   u16 scora=0;
   scora=Get_Level_Scores();
   if(Scores>=scora)
   {
      Level++;
      scora=Get_Level_Scores();
      Show_Level();
      Level_Up=scora-Scores;
      Show_Scores();
      TIM3_Init(Vel,7199);
   }
}

void Show_Level(void)
{
   u8 te;
   u16 tx=32,ty=144,Temp[12];
   u8 level_buf[3];
   level_buf[0]=Level/10+0x30;
   level_buf[1]=Level%10+0x30;
   level_buf[2]='\0';
   for(te=0;te<12;te++)
   {
      Temp[te]=LCD_ReadPoint(tx,ty);
      tx+=16;
      if(tx-32==96)
      {
         tx=32;
         ty+=16;
      }
   }
   GUI_Box(32,144,32+95,144+31,GREEN);
  
   GUI_Show12ASCII(48,152,"LEVEL   ",RED,GREEN);
   GUI_Show12ASCII(96,152,level_buf,RED,GREEN);
   delay_ms(1000);
   delay_ms(1000);
 //  GUI_Box(32,144,32+95,144+31,WHITE);
   tx=32;
   ty=144;
   for(te=0;te<12;te++)
   {
      POINT_COLOR=Temp[te];
      Draw_Block(tx,ty,16,16);
      tx+=16;
      if(tx-32==96)
      {
         tx=32;
         ty+=16;
      }
   }
}

