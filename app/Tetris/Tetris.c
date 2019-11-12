#include "tetris.h"
#include "tftlcd.h"
#include "stdlib.h"
#include "picture.h"
#include "ascii.h" 


u16 Tetris[19]={0x0F00,0x4444,0x0660,0x4460,0x02E0,0x6220,0x0740,0x2260,0x0E20,0x6440,0x0470,0x0C60,0x2640,0x0360,0x4620,0x04E0,0x2620,0x0E40,0x4640};
u16 BoxSR[Y_BOXS+4]={0x0000,0x0000,0x0000,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0x8001,0xFFFF};

_Shape NewShape={60,30,SHAPE_COLOR,1,1,1,0};
_Game Game={10000,1,0};



//画矩形	  
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine_Color(x1,y1,x2,y1,color);
	LCD_DrawLine_Color(x1,y1,x1,y2,color);
	LCD_DrawLine_Color(x1,y2,x2,y2,color);
	LCD_DrawLine_Color(x2,y1,x2,y2,color);
}

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


//在指定位置 显示1个16*16的汉字
//(x,y):汉字显示的位置
//index:tfont数组里面的第几个汉字
//color:这个汉字的颜色
void Test_Show_CH_Font16(u16 x,u16 y,u8 index,u16 color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;				   
    for(t=0;t<32;t++)//每个16*16的汉字点阵 有32个字节
    {   
		if(t<16)temp=tfont16[index*2][t];      //前16个字节
		else temp=tfont16[index*2+1][t-16];    //后16个字节	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_DrawFRONT_COLOR(x,y,color);//画实心点
			else LCD_DrawFRONT_COLOR(x,y,GBLUE);   //画空白点（使用背景色）
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


/*LCD画小方块函数**/
/*(sx,xy):起始坐标*/
/***color:方块颜色*/
void LCD_DrawBlock(u16 sx,u16 sy,u16 color)
{
	LCD_DrawRectangle_Color(sx,sy,sx+BPIXEL-1,sy+BPIXEL-1,color);	
	LCD_Fill(sx+2,sy+2,sx+BPIXEL-3,sy+BPIXEL-3,color);
}

/*********绘制图形函数************/
/*(sx,sy):基点，在4*4区域的左上角*/
/*******n:基本图形编号************/
/***color:颜色值******************/
void LCD_DrawShape(u16 sx,u16 sy,u8 n,u16 color)
{
	u8 a,b;
	u16 temp=Tetris[n];
	u8 i;
	
	for(i=0;i<16;i++)
	{
		a=i/4;
		b=i%4;
		if(temp&0x8000)
		{
			LCD_DrawRectangle_Color(sx+b*BPIXEL,sy+a*BPIXEL,sx+(b+1)*BPIXEL-1,sy+(a+1)*BPIXEL-1,color);
			LCD_Fill(sx+b*BPIXEL+2,sy+a*BPIXEL+2,sx+(b+1)*BPIXEL-3,sy+(a+1)*BPIXEL-3,color);
			BoxSR[sy/BPIXEL+a]|=1<<((sx/BPIXEL)+b);	//标记该方块已被图形占用
		}
		temp<<=1;
	}
}

/*********消除图形函数***************/
/*(sx,sy):基点，在4*4区域的左上角****/
/*******n:基本图形编号***************/
/***color:颜色值*********************/
/*消除图形时应将相应方块的标志位清零*/
void LCD_ClearShape(u16 sx,u16 sy,u8 n,u16 color)
{
	u8 a,b;
	u16 temp=Tetris[n];
	u8 i;
	for(i=0;i<16;i++)
	{
		a=i/4;
		b=i%4;
		if(temp&0x8000)
		{
			LCD_Fill(sx+b*BPIXEL,sy+a*BPIXEL,sx+(b+1)*BPIXEL-1,sy+(a+1)*BPIXEL-1,color);
			BoxSR[sy/BPIXEL+a]&=~(1<<((sx/BPIXEL)+b));	//标记该方块未被图形占用
		}
		temp<<=1;
	}		
}

/***********************判断函数***********************/
/*****************判断移动图形的可行性*****************/
/*mode:1->左移;2->右移;3->旋转;4->加速下落;5->自由下落*/
u8 Judge(u16 sx,u16 sy,u8 n,u8 mode)
{
	int cx,cy,temp1=Tetris[n],temp2=Tetris[n];
	u8 a,b,i,Flag=0;
	switch(mode)
	{
		case 1:	cx=sx-BPIXEL;cy=sy;break;
		case 2:	cx=sx+BPIXEL;cy=sy;break;
		case 3:	cx=sx;
				cy=sy;
				switch(NewShape.CurNum)
				{
					case 0:temp2=1;break;
					case 1:temp2=0;break;
					case 2:temp2=2;break;
					case 3:temp2=4;break;
					case 4:temp2=5;break;
					case 5:temp2=6;break;
					case 6:temp2=3;break;
					case 7:temp2=8;break;
					case 8:temp2=9;break;
					case 9:temp2=10;break;
					case 10:temp2=7;break;
					case 11:temp2=12;break;
					case 12:temp2=11;break;
					case 13:temp2=14;break;
					case 14:temp2=13;break;
					case 15:temp2=16;break;
					case 16:temp2=17;break;
					case 17:temp2=18;break;
					case 18:temp2=15;break;
				}
				NewShape.TurnNum=temp2;
				temp2=Tetris[temp2];
				break;
		case 5:	cx=sx;cy=sy+BPIXEL;break;
		default:cx=sx;cy=sy;break;					
	}
	for(i=0;i<16;i++)
	{
		a=i/4;	
		b=i%4;
		if(temp1&0x8000)BoxSR[sy/BPIXEL+a]&=~(1<<((sx/BPIXEL)+b));
		temp1<<=1;
	}
	for(i=0;i<16;i++)
	{
		a=i/4;
		b=i%4;
		if(temp2&0x8000)
		{
			if(BoxSR[cy/BPIXEL+a]&(1<<((cx/BPIXEL)+b)))Flag=1;
		}
		temp2<<=1;			
	}	
	if(Flag==0)return 1;
	else return 0;
}

/*创建新图形函数*/
void Create_Shape(void)
{
	u8 a,b;
	u16 temp;
	u8 i;
	NewShape.CurNum=NewShape.NextNum;
	NewShape.x=60;
	NewShape.y=30;
	if(BoxSR[4]&0x03C0)ResetGame();
	else	LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
	temp=Tetris[NewShape.NextNum];
	for(i=0;i<16;i++)
	{
		a=i/4;
		b=i%4;
		if(temp&0x8000)
		{
			LCD_DrawBlock(180+b*BPIXEL,50+a*BPIXEL,WILL_COLOR);
		}
		temp<<=1;
	}
	NewShape.NextNum=rand()%19;
	temp=Tetris[NewShape.NextNum];
	for(i=0;i<16;i++)
	{
		a=i/4;
		b=i%4;
		if(temp&0x8000)
		{
			LCD_DrawBlock(180+b*BPIXEL,50+a*BPIXEL,SHAPE_COLOR);
		}
		temp<<=1;
	}
}

/*图形左移函数*/
void MoveLeft(void)
{
	u8 Draw_Ready;
	if(NewShape.Move==0)
	{
		NewShape.Move=1;
		Draw_Ready=Judge(NewShape.x,NewShape.y,NewShape.CurNum,1);
		if(Draw_Ready==1)
		{
			LCD_ClearShape(NewShape.x,NewShape.y,NewShape.CurNum,BOXS_COLOR);
			NewShape.x-=BPIXEL;
			LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		}
		else LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		NewShape.Move=0;
	}
}

/*图形右移函数*/
void MoveRight(void)
{
	u8 Draw_Ready;
	if(NewShape.Move==0)
	{
		NewShape.Move=1;
		Draw_Ready=Judge(NewShape.x,NewShape.y,NewShape.CurNum,2);
		if(Draw_Ready==1)
		{
			LCD_ClearShape(NewShape.x,NewShape.y,NewShape.CurNum,BOXS_COLOR);
			NewShape.x+=BPIXEL;
			LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		}
		else LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		NewShape.Move=0;
	}
}

/*快速下落函数*/
void DownFast(void)
{
	u8 Draw_Ready;
	if(NewShape.Move==0)
	{
		NewShape.Move=1;
		Draw_Ready=Judge(NewShape.x,NewShape.y,NewShape.CurNum,5);
		if(Draw_Ready==1)
		{
			LCD_ClearShape(NewShape.x,NewShape.y,NewShape.CurNum,BOXS_COLOR);
			NewShape.y+=BPIXEL;
			LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		}
		else
		{
			LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		}
		NewShape.Move=0;
	}
}
u8 score_buf[7],level_buf[4];
/*自由下落函数*/
void DownFree(void)
{
	u8 Draw_Ready;
	u8 i,j,Clear_Flag;
	u16 temp;
	
	if(NewShape.Move==0)
	{
		NewShape.Move=1;
		Draw_Ready=Judge(NewShape.x,NewShape.y,NewShape.CurNum,5);
		if(Draw_Ready==1)
		{
			LCD_ClearShape(NewShape.x,NewShape.y,NewShape.CurNum,BOXS_COLOR);
			NewShape.y+=BPIXEL;
			LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		}
		else
		{
			LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
			for(i=4;i<Y_BOXS+4-1;i++)
			{
				if(BoxSR[i]==0xFFFF)
				{
					Game.score++;
					Game.level=1+Game.score/100;

					score_buf[0]=Game.score/100000+0x30;
					score_buf[1]=Game.score%100000/10000+0x30;
					score_buf[2]=Game.score%100000%10000/1000+0x30;
					score_buf[3]=Game.score%100000%10000%1000/100+0x30;
					score_buf[4]=Game.score%100000%10000%1000%100/10+0x30;
					score_buf[5]=Game.score%100000%10000%1000%100%10+0x30;
					score_buf[6]='\0';
				
					level_buf[0]=Game.level/100+0x30;
					level_buf[1]=Game.level%100/10+0x30;
					level_buf[2]=Game.level%100%10+0x30;
					level_buf[3]='\0';
					
					FRONT_COLOR=TEXT_COLOR;
					BACK_COLOR=GBLUE;
					LCD_ShowString(170,120,tftlcd_data.width,tftlcd_data.height,16,score_buf);
					LCD_ShowString(170,160,tftlcd_data.width,tftlcd_data.height,16,level_buf);
					Clear_Flag=1;
					for(j=i;j>3;j--)BoxSR[j]=BoxSR[j-1];
				}		
			}
			if(Clear_Flag)LCD_Fill(10,30,149,279,BOXS_COLOR);
			for(i=3;i<Y_BOXS+4-1;i++)
			{
				temp=BoxSR[i];
				temp>>=1;
				for(j=1;j<15;j++)
				{
					if(temp&0x0001)LCD_DrawBlock(j*BPIXEL,i*BPIXEL,SHAPE_COLOR);
					temp>>=1;
				}
			}
			Create_Shape();
		}
		NewShape.Move=0;
	}
}

void Transform(void)
{
	u8 Draw_Ready;
	if(NewShape.Move==0)
	{
		NewShape.Move=1;
		Draw_Ready=Judge(NewShape.x,NewShape.y,NewShape.CurNum,3);
		if(Draw_Ready==1)
		{
			LCD_ClearShape(NewShape.x,NewShape.y,NewShape.CurNum,BOXS_COLOR);
			NewShape.CurNum=NewShape.TurnNum;
			LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		}
		else LCD_DrawShape(NewShape.x,NewShape.y,NewShape.CurNum,SHAPE_COLOR);
		NewShape.Move=0;
	}
}
	

void Show_TetrisFace(void)
{
	u8 i,j;
	u16 curpos=30;
	LCD_Clear(BOXS_COLOR);
	
	Test_Show_CH_Font16(0+50,0+10,19,TEXT_COLOR);	    //俄
	Test_Show_CH_Font16(16+50,0+10,20,TEXT_COLOR);	//罗
	Test_Show_CH_Font16(32+50,0+10,21,TEXT_COLOR);	//斯
	Test_Show_CH_Font16(48+50,0+10,22,TEXT_COLOR);	//方
	Test_Show_CH_Font16(64+50,0+10,23,TEXT_COLOR);	//块
	
	Test_Show_CH_Font16(160,30,11,TEXT_COLOR);	//下
	Test_Show_CH_Font16(176,30,12,TEXT_COLOR);	//一
	Test_Show_CH_Font16(192,30,13,TEXT_COLOR);	//个
	
	Test_Show_CH_Font16(160,100,7,TEXT_COLOR);	//分
	Test_Show_CH_Font16(176,100,8,TEXT_COLOR);	//数
	
	Test_Show_CH_Font16(160,140,9,TEXT_COLOR);	//等
	Test_Show_CH_Font16(176,140,10,TEXT_COLOR);	//级
	
	Test_Show_CH_Font16(170,210,24,TEXT_COLOR);//按
	Test_Show_CH_Font16(210,210,25,TEXT_COLOR);//键
	Test_Show_CH_Font16(190,190,26,TEXT_COLOR);//信
	Test_Show_CH_Font16(190,230,27,TEXT_COLOR);//息

	FRONT_COLOR=RED;
	BACK_COLOR=GBLUE;
	LCD_ShowString(10,300,tftlcd_data.width,tftlcd_data.height,16,"K_UP:TURN");
	LCD_ShowString(10,320,tftlcd_data.width,tftlcd_data.height,16,"K_DOWN:DOWN");
	LCD_ShowString(10,340,tftlcd_data.width,tftlcd_data.height,16,"K_RIGHT:RIGHT");
	LCD_ShowString(10,360,tftlcd_data.width,tftlcd_data.height,16,"K_LEFT:LEFT");
	
	FRONT_COLOR=TEXT_COLOR;
	LCD_ShowString(170,120,tftlcd_data.width,tftlcd_data.height,16,score_buf);
	LCD_ShowString(170,160,tftlcd_data.width,tftlcd_data.height,16,level_buf);
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			LCD_DrawBlock(180+i*BPIXEL,50+j*BPIXEL,WILL_COLOR);
		}
	}
	
	for(i=0;i<25;i++)
	{
		LCD_DrawBlock(0,curpos,WILL_COLOR);
		curpos+=BPIXEL;	
	}
	
	curpos=30;
	for(i=0;i<25;i++)
	{
		LCD_DrawBlock(150,curpos,WILL_COLOR);
		curpos+=BPIXEL;	
	}
	
	curpos=0;
	for(i=0;i<16;i++)
	{
		LCD_DrawBlock(curpos,280,WILL_COLOR);
		curpos+=BPIXEL;	
	}
}

void Start_Game(void)
{
	Create_Shape();	
}

void ResetGame(void)
{
	u8 i;
	LCD_Fill(10,30,149,279,BOXS_COLOR);
	Game.score=0;
	Game.level=1;
	BoxSR[0]=0x0000;
	BoxSR[1]=0x0000;
	BoxSR[2]=0x0000;
	for(i=3;i<Y_BOXS+4-1;i++)
	{
		BoxSR[i]=0x8001;
	}
	BoxSR[Y_BOXS+4-1]=0xFFFF;
	
	Start_Game();	
}
	

