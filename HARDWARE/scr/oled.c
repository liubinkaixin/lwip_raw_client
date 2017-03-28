/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/
/*-------------------------------------------------*/
/*            延时源文件,有10个函数                */
/*-------------------------------------------------*/

/*-------------------------------------------------*/

#include "oled.h"
#include "oled_font.h"
#include "delay.h"

/*-------------------------------------------------*/
/*函数名：OLED写数据函数模拟SPI                    */
/*参  数：dat：数据                                */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_WrDat(u8 dat)     
{
    u8 i=8, temp=0;
    OLED_DC=1;  
    for(i=0;i<8;i++) //发送一个八位数据 
    {
        OLED_SCL=0;  
        
        temp = dat&0x80;
        if (temp == 0)
        {
            OLED_SDA = 0;
        }
        else
        {
            OLED_SDA = 1;
        }
        OLED_SCL=1;             
        dat<<=1;    
    }
}

/*-------------------------------------------------*/
/*函数名：OLED写命令模拟SPI                        */
/*参  数：cmd：命令                                */
/*返回值：无                                       */
/*-------------------------------------------------*/                                     
void OLED_WrCmd(u8 cmd)
{
    u8 i=8, temp=0;
    OLED_DC=0;
    for(i=0;i<8;i++) //发送一个八位数据 
    { 
        OLED_SCL=0; 
       
        temp = cmd&0x80;
        if (temp == 0)
        {
            OLED_SDA = 0;
        }
        else
        {
            OLED_SDA = 1;
        }
        OLED_SCL=1;
        cmd<<=1;;        
    }     
}

/*-------------------------------------------------*/
/*函数名：OLED 设置坐标                            */
/*参  数：x：坐标    y：坐标                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_Set_Pos(u8 x, u8 y) 
{ 
    OLED_WrCmd(0xb0+y);
    OLED_WrCmd(((x&0xf0)>>4)|0x10);
    OLED_WrCmd((x&0x0f)|0x01); 
} 

/*-------------------------------------------------*/
/*函数名：OLED全屏显示                             */
/*参  数：bmp_dat：数据                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_Fill(u8 bmp_dat) 
{
    u8 y,x;
    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x=0;x<X_WIDTH;x++)
            OLED_WrDat(bmp_dat);
    }
}

/*-------------------------------------------------*/
/*函数名：OLED复位                                 */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_CLS(void)
{
    u8 y,x;    
    for(y=0;y<8;y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10); 
        for(x=0;x<X_WIDTH;x++)
            OLED_WrDat(0);
    }
}

/*-------------------------------------------------*/
/*函数名：OLED初始化                               */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_Init(void)     
{  
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;       //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);               //初始化PF6/7/8/9
    GPIO_SetBits(GPIOF,GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);	
	
    delay_ms(500);
    OLED_CS=0; 
   
    OLED_WrCmd(0xae);
    OLED_WrCmd(0xae);//--turn off oled panel
    OLED_WrCmd(0x00);//---set low column address
    OLED_WrCmd(0x10);//---set high column address
    OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WrCmd(0x81);//--set contrast control register
    OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
    OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0,0xa1
    OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0,0xc8
    OLED_WrCmd(0xa6);//--set normal display
    OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    OLED_WrCmd(0x3f);//--1/64 duty
    OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WrCmd(0x00);//-not offset
    OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_WrCmd(0xd9);//--set pre-charge period
    OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WrCmd(0xda);//--set com pins hardware configuration
    OLED_WrCmd(0x12);
    OLED_WrCmd(0xdb);//--set vcomh
    OLED_WrCmd(0x40);//Set VCOM Deselect Level
    OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_WrCmd(0x02);//
    OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
    OLED_WrCmd(0x14);//--set(0x10) disable
    OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
    OLED_WrCmd(0xaf);//--turn on oled panel
    OLED_Fill(0x00);
    OLED_Set_Pos(0,0);    
} 

/*-------------------------------------------------*/
/*函数名：显示6*8ASCII字符串                       */
/*参  数：x:坐标                                   */
/*参  数：y:坐标                                   */
/*参  数：ch[]:数据字符                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_P6x8Str(u8 x, u8 y,u8 ch[])
{
    u8 c=0,i=0,j=0;      
    while (ch[j]!='\0')
    {    
        c =ch[j]-32;
        if(x>126){x=0;y++;}
        OLED_Set_Pos(x,y);    
        for(i=0;i<6;i++)     
            OLED_WrDat(F6x8[c][i]);  
        x+=6;
        j++;
    }
}

/*-------------------------------------------------*/
/*函数名：显示8*16ASCII字符串                      */
/*参  数：x:坐标                                   */
/*参  数：y:坐标                                   */
/*参  数：ch[]:数据字符                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_P8x16Str(u8 x, u8 y,u8 ch[])
{
    u8 c=0,i=0,j=0;
    while (ch[j]!='\0')
    {    
        c =ch[j]-32;
        if(x>120){x=0;y++;}
        OLED_Set_Pos(x,y);    
        for(i=0;i<8;i++)     
            OLED_WrDat(F8X16[c*16+i]);
        OLED_Set_Pos(x,y+1);    
        for(i=0;i<8;i++)     
            OLED_WrDat(F8X16[c*16+i+8]);  
        x+=8;
        j++;
    }
}

/*-------------------------------------------------*/
/*函数名：显示16*16点阵                            */
/*参  数：x:坐标                                   */
/*参  数：y:坐标                                   */
/*参  数：N                                        */
/*返回值：无                                       */
/*-------------------------------------------------*/
void OLED_P16x16Ch(u8 x, u8 y, u8 N)
{
    u8 wm=0;
    unsigned int adder=32*N;  //      
    OLED_Set_Pos(x , y);
    for(wm = 0;wm < 16;wm++)  //             
    {
        OLED_WrDat(F16x16[adder]);    
        adder += 1;
    }      
    OLED_Set_Pos(x,y + 1);    
    for(wm = 0;wm < 16;wm++) //         
    {
        OLED_WrDat(F16x16[adder]);
        adder += 1;
    }           
}

/*-------------------------------------------------*/
/*函数名：显示BMP图片                              */
/*参  数：x0:坐标                                  */
/*参  数：y0:坐标                                  */
/*参  数：x1:坐标                                  */
/*参  数：y1:坐标                                  */
/*参  数：BMP：图片数据                            */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Draw_BMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[])
{     
    unsigned int j=0;
    u8 x,y;
    
    if(y1%8==0) y=y1/8;      
    else y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_Pos(x0,y);                
        for(x=x0;x<x1;x++)
        {      
            OLED_WrDat(BMP[j++]);            
        }
    }
} 
