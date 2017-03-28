/*-------------------------------------------------*/
/*         超纬电子STM32F407ZGT6开发板             */
/*    淘宝地址： http://chaoweidianzi.taobao.com   */
/*-------------------------------------------------*/

#ifndef __OLED_H
#define __OLED_H

#include "main.h"

#define OLED_SCL PFout(6)       //D0
#define OLED_SDA PFout(7)       //D1
#define OLED_DC  PFout(8)       //DC
#define OLED_CS  PFout(9)       //CS

#define XLevelL        0x00
#define XLevelH        0x10
#define XLevel         ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column     128
#define Max_Row        64
#define Brightness     0xCF 
#define X_WIDTH        128
#define Y_WIDTH        64

void OLED_WrDat(u8 dat);
void OLED_WrCmd(u8 cmd);
void OLED_Set_Pos(u8 x, u8 y) ;
void OLED_Fill(u8 bmp_dat) ;
void OLED_CLS(void);
void OLED_Init(void) ;
void OLED_P6x8Str(u8 x, u8 y,u8 ch[]);
void OLED_P8x16Str(u8 x, u8 y,u8 ch[]);
void OLED_P16x16Ch(u8 x, u8 y, u8 N);
void Draw_BMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);


#endif
