#include "touch.h"
#include "24cxx.h"
#include "SysTick.h"
#include "tftlcd.h"
#include "spi.h"


#define TOUCH_AdjDelay500ms() delay_ms(500)

TouchTypeDef TouchData;         //定义用来存储读取到的数据
static PosTypeDef TouchAdj;     //定义一阵数据用来保存校正因数



void TOUCH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* SPI的IO口和SPI外设打开时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    /* TOUCH-CS的IO口设置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* TOUCH-PEN的IO口设置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    SPI1_Init();

    /* 使用EEPROM来存储校正参数，所以注意之前要初始化 */
    /* 检测是否有校正参数 */
    AT24CXX_Init();		//初始化24CXX
	AT24CXX_Read(TOUCH_ADJ_ADDR,&TouchAdj.posState,sizeof(TouchAdj));
	if(TouchAdj.posState != TOUCH_ADJ_OK)
    {
        TOUCH_Adjust(); //校正   
    }
}



uint16_t TOUCH_ReadData(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t readValue[TOUCH_READ_TIMES], value;
    uint32_t totalValue;
	
	/* SPI的速度不宜过快 */
    SPI1_SetSpeed(SPI_BaudRatePrescaler_16);
	
    /* 读取TOUCH_READ_TIMES次触摸值 */
    for(i=0; i<TOUCH_READ_TIMES; i++)
    {   /* 打开片选 */
        TCS=0;
        /* 在差分模式下，XPT2046转换需要24个时钟，8个时钟输入命令，之后1个时钟去除 */
        /* 忙信号，接着输出12位转换结果，剩下3个时钟是忽略位 */    
        SPI1_ReadWriteByte(cmd); // 发送命令，选择X轴或者Y轴 
        
        /* 读取数据 */
        readValue[i] = SPI1_ReadWriteByte(0xFF);
        readValue[i] <<= 8;
        readValue[i] |= SPI1_ReadWriteByte(0xFF);
        
        /* 将数据处理，读取到的AD值的只有12位，最低三位无用 */
        readValue[i] >>= 3;
        
        TCS=1;
    }

    /* 滤波处理 */
    /* 首先从大到小排序 */
    for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
    {
        for(j=i+1; j<TOUCH_READ_TIMES; j++)
        {
            /* 采样值从大到小排序排序 */
            if(readValue[i] < readValue[j])
            {
                value = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = value;
            }   
        }       
    }
   
    /* 去掉最大值，去掉最小值，求平均值 */
    j = TOUCH_READ_TIMES - 1;
    totalValue = 0;
    for(i=1; i<j; i++)     //求y的全部值
    {
        totalValue += readValue[i];
    }
    value = totalValue / (TOUCH_READ_TIMES - 2);
      
    return value;
}

uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{   
    uint16_t xValue1, yValue1, xValue2, yValue2;

    xValue1 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue1 = TOUCH_ReadData(TOUCH_Y_CMD);
    xValue2 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue2 = TOUCH_ReadData(TOUCH_Y_CMD);
    
    /* 查看两个点之间的只采样值差距 */
    if(xValue1 > xValue2)
    {
        *xValue = xValue1 - xValue2;
    }
    else
    {
        *xValue = xValue2 - xValue1;
    }

    if(yValue1 > yValue2)
    {
        *yValue = yValue1 - yValue2;
    }
    else
    {
        *yValue = yValue2 - yValue1;
    }

    /* 判断采样差值是否在可控范围内 */
	if((*xValue > TOUCH_MAX+0) || (*yValue > TOUCH_MAX+0))  
	{
		return 0xFF;
	}

    /* 求平均值 */
    *xValue = (xValue1 + xValue2) / 2;
    *yValue = (yValue1 + yValue2) / 2;

    /* 判断得到的值，是否在取值范围之内 */
    if((*xValue > TOUCH_X_MAX+0) || (*xValue < TOUCH_X_MIN) 
       || (*yValue > TOUCH_Y_MAX+0) || (*yValue < TOUCH_Y_MIN))
    {                   
        return 0xFF;
    }
 
    return 0; 
}

uint8_t TOUCH_ReadAdjust(uint16_t x, uint16_t y, uint16_t *xValue, uint16_t *yValue)
{
    uint8_t i;
    uint32_t timeCont;

    /* 读取校正点的坐标 */
    LCD_Clear(BACK_COLOR);
    LCD_DrowSign(x, y, RED);
    i = 0;
    while(1)
    {
        if(!TOUCH_ReadXY(xValue, yValue))
        {
            i++;
            if(i > 10)         //延时一下，以读取最佳值
            {
                LCD_DrowSign(x, y, BACK_COLOR);
                return 0;
            }
               
        }
		timeCont++;
        /* 超时退出 */
        if(timeCont > 0xFFFFFFFE)
        {   
                LCD_DrowSign(x, y, BACK_COLOR); 
                return 0xFF;
        } 
    }       
}

void TOUCH_Adjust(void)
{
    uint16_t px[2], py[2], xPot[4], yPot[4];
    float xFactor, yFactor;

    /* 读取第一个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MIN, &xPot[0], &yPot[0]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* 读取第二个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MAX, &xPot[1], &yPot[1]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* 读取第三个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MIN, &xPot[2], &yPot[2]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* 读取第四个点 */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MAX, &xPot[3], &yPot[3]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();
    
    /* 处理读取到的四个点的数据，整合成对角的两个点 */
    px[0] = (xPot[0] + xPot[1]) / 2;
    py[0] = (yPot[0] + yPot[2]) / 2;
    px[1] = (xPot[3] + xPot[2]) / 2;
    py[1] = (yPot[3] + yPot[1]) / 2;

    /* 求出比例因数 */
    xFactor = (float)LCD_ADJ_X / (px[1] - px[0]);
    yFactor = (float)LCD_ADJ_Y / (py[1] - py[0]);  
    
    /* 求出偏移量 */
    TouchAdj.xOffset = (int16_t)LCD_ADJX_MAX - ((float)px[1] * xFactor);
    TouchAdj.yOffset = (int16_t)LCD_ADJY_MAX - ((float)py[1] * yFactor);

    /* 将比例因数进行数据处理，然后保存 */
    TouchAdj.xFactor = xFactor ;
    TouchAdj.yFactor = yFactor ;
    
    TouchAdj.posState = TOUCH_ADJ_OK;
    AT24CXX_Write(TOUCH_ADJ_ADDR, &TouchAdj.posState, sizeof(TouchAdj));            
}


uint8_t TOUCH_Scan(void)
{
    
//    if(PEN == 0)   //查看是否有触摸
    {
        if(TOUCH_ReadXY(&TouchData.x, &TouchData.y)) //没有触摸
        {
            return 0xFF;    
        }
        /* 根据物理坐标值，计算出彩屏坐标值 */
        TouchData.lcdx = TouchData.x * TouchAdj.xFactor + TouchAdj.xOffset;
        TouchData.lcdy = TouchData.y * TouchAdj.yFactor + TouchAdj.yOffset;
        
        /* 查看彩屏坐标值是否超过彩屏大小 */
        if(TouchData.lcdx > tftlcd_data.width)
        {
            TouchData.lcdx = tftlcd_data.width;
        }
        if(TouchData.lcdy > tftlcd_data.height)
        {
            TouchData.lcdy = tftlcd_data.height;
        }
        return 0; 
    }
 //   return 0xFF;       
}







