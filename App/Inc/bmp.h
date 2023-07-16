//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   :
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明:
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PD6（SCL）
//              D1   接PD7（SDA）
//              RES  接PD4
//              DC   接PD5
//              CS   接PD3
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   :
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/

#ifndef __BMP_H
#define __BMP_H
unsigned char BMP1[] =
{
    0x00, 0x03, 0x05, 0x09, 0x11, 0xFF, 0x11, 0x89, 0x05, 0xC3, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0xF8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0xFF, 0x11, 0xAA, 0x44, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x01, 0x38, 0x44, 0x82, 0x92,
    0x92, 0x74, 0x01, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x44, 0xFF, 0x01, 0x7D,
    0x7D, 0x7D, 0x01, 0x7D, 0x7D, 0x7D, 0x7D, 0x01, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x01, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x03, 0x03,
    0xF3, 0x13, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x01, 0xF1, 0x11, 0x61, 0x81, 0x01, 0x01, 0x01,
    0x81, 0x61, 0x11, 0xF1, 0x01, 0x01, 0x01, 0x01, 0x41, 0x41, 0xF1, 0x01, 0x01, 0x01, 0x01, 0x01,
    0xC1, 0x21, 0x11, 0x11, 0x11, 0x11, 0x21, 0xC1, 0x01, 0x01, 0x01, 0x01, 0x41, 0x41, 0xF1, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x11, 0x11, 0x11, 0x11, 0x11, 0xD3, 0x33,
    0x03, 0x03, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0x00, 0x00,
    0x7F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x01, 0x06, 0x18, 0x06,
    0x01, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x00, 0x00, 0x00,
    0x1F, 0x20, 0x40, 0x40, 0x40, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x40, 0x40, 0x7F, 0x40,
    0x40, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x40, 0x30, 0x0C, 0x03, 0x00, 0x00,
    0x00, 0x00, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x06, 0x06,
    0x06, 0x06, 0x04, 0x04, 0x04, 0x84, 0x44, 0x44, 0x44, 0x84, 0x04, 0x04, 0x84, 0x44, 0x44, 0x44,
    0x84, 0x04, 0x04, 0x04, 0x84, 0xC4, 0x04, 0x04, 0x04, 0x04, 0x84, 0x44, 0x44, 0x44, 0x84, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x84, 0x44, 0x44, 0x44, 0x84, 0x04, 0x04, 0x04, 0x04, 0x04, 0x84, 0x44,
    0x44, 0x44, 0x84, 0x04, 0x04, 0x84, 0x44, 0x44, 0x44, 0x84, 0x04, 0x04, 0x04, 0x04, 0x06, 0x06,
    0x06, 0x06, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x18, 0x14, 0x12, 0x11, 0x00, 0x00, 0x0F, 0x10, 0x10, 0x10,
    0x0F, 0x00, 0x00, 0x00, 0x10, 0x1F, 0x10, 0x00, 0x00, 0x00, 0x08, 0x10, 0x12, 0x12, 0x0D, 0x00,
    0x00, 0x18, 0x00, 0x00, 0x0D, 0x12, 0x12, 0x12, 0x0D, 0x00, 0x00, 0x18, 0x00, 0x00, 0x10, 0x18,
    0x14, 0x12, 0x11, 0x00, 0x00, 0x10, 0x18, 0x14, 0x12, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x7F, 0x03, 0x0C, 0x30, 0x0C, 0x03, 0x7F, 0x00, 0x00, 0x38, 0x54, 0x54, 0x58, 0x00, 0x00,
    0x7C, 0x04, 0x04, 0x78, 0x00, 0x00, 0x3C, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xAA, 0xAA, 0xAA,
    0x28, 0x08, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x03, 0x0C, 0x30, 0x0C, 0x03, 0x7F,
    0x00, 0x00, 0x26, 0x49, 0x49, 0x49, 0x32, 0x00, 0x00, 0x7F, 0x02, 0x04, 0x08, 0x10, 0x7F, 0x00, /********************************/
};
unsigned char BMP[] = 
{
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x1F,0x0F,
0x0F,0x07,0x07,0x07,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x07,0x07,0x8F,0xCF,
0xCF,0xEF,0xEF,0xEF,0xF7,0xF7,0xF7,0xFF,0xFB,0xFB,0xFB,0xFF,0xFF,0xFF,0xFD,0xFD,
0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFD,0xFB,
0xFB,0xFB,0xFB,0xFB,0xFB,0xFB,0xFF,0xF7,0xF7,0xE7,0xE7,0xC3,0x83,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x07,0x07,0x0F,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF0,0xF8,0xFE,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,
0xFC,0xF8,0xF0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xF0,0xFE,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0x1F,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0xC0,0xE0,0xE0,0xF0,0xF1,0xF9,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,
0xFE,0xFC,0xFC,0x18,0x00,0xF0,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF2,
0xF2,0xF0,0xD8,0x98,0x98,0x80,0xC1,0xC1,0xC3,0xE3,0xE7,0xFF,0xFF,0xFB,0xFB,0xF3,
0xF3,0xF3,0xE1,0xE1,0xC0,0xD0,0x98,0x98,0xB8,0xB2,0xE2,0xE6,0xE7,0xE7,0xE7,0xE7,
0xE7,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xE4,0xCE,0x3E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x8F,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x7F,0x7F,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,
0x3F,0x1F,0x07,0x08,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xBF,0xFF,0xFF,0xFF,0xBF,0xBF,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0xBF,0xBF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xCF,0x9F,0x9F,0x3F,0x3F,0x7F,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0x03,0x0F,0x1F,0x1F,0x3F,
0xFF,0xFF,0xFF,0xFF,0xFE,0xFC,0xF8,0xF0,0xF0,0xF0,0xE0,0xE0,0xC0,0xC0,0xC0,0x80,
0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
0x01,0x03,0x03,0x03,0x03,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x01,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x0F,0x3F,0x3F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xE7,0xE7,0xE7,0xE7,0xE7,0xEF,0xFF,0xFF,0xF7,
0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x7F,0x7F,0x3F,0x3F,0x1F,0x0F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFE,0xFE,0xFC,0xF8,0xF8,0xF0,0xE0,0xE0,0xC0,0xC0,0x80,0x80,0x80,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x03,0x07,0x07,0x07,
0x07,0x07,0x0F,0x0F,0x0F,0x0F,0x1F,0x1F,0x1C,0x1C,0x1C,0x3C,0x38,0x38,0x3C,0x3C,
0x3C,0x3E,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,
0x3F,0x3F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x0F,0x0F,0x07,0x07,0x03,0x03,0x03,
0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xBF,0xBF,0xAF,0xAF,0xAF,0x87,0x07,
0x2F,0xAF,0xAF,0x9F,0x9F,0xAF,0xBF,0xFF,0xFF,0xFF,0xAF,0xAF,0x37,0x37,0x67,0x4F,
0x5F,0x5F,0x5F,0x6F,0x37,0x37,0xB7,0xAF,0xFF,0xFF,0xFF,0x1F,0x4F,0xE7,0xE7,0xEF,
0x0F,0xFF,0x9F,0x47,0xE7,0xEF,0xEF,0xEF,0x0F,0xFF,0xFF,0xFF,0xDF,0xDF,0x47,0x4F,
0x7F,0x5F,0x5F,0x47,0x57,0x7F,0x7F,0x7F,0x7F,0xDF,0xFF,0xFF,0xFF,0xAF,0x8F,0xAF,
0xAF,0xAF,0xAF,0xAF,0x27,0xAF,0xAF,0xAF,0xAF,0xAF,0x8F,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD,0xFD,0xFF,0xF0,0xF0,0xEE,0xED,
0xED,0xED,0xED,0xEF,0xF7,0xF3,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xF0,0xED,0xED,0xED,
0xED,0xEC,0xED,0xED,0xED,0xED,0xEF,0xF3,0xFF,0xFF,0xFF,0xE0,0xF6,0xF7,0xF7,0xF7,
0xE0,0xFF,0xFF,0xFF,0xEE,0xEE,0xEF,0xF7,0xF0,0xFF,0xFF,0xFF,0xFF,0xFF,0xE0,0xFB,
0xFB,0xFB,0xFB,0xFB,0xFB,0xEB,0xEB,0xE9,0xF0,0xFF,0xFF,0xFF,0xFF,0xEF,0xF7,0xF9,
0xFA,0xF7,0xF7,0xEF,0xE0,0xED,0xED,0xED,0xED,0xED,0xEF,0xEF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

unsigned char BMP2[] =
{
    0x00, 0x03, 0x05, 0x09, 0x11, 0xFF, 0x11, 0x89, 0x05, 0xC3, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0xF8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0xFF, 0x11, 0xAA, 0x44, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x01, 0x38, 0x44, 0x82, 0x92,
    0x92, 0x74, 0x01, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x44, 0xFF, 0x01, 0x7D,
    0x7D, 0x7D, 0x7D, 0x01, 0x7D, 0x7D, 0x7D, 0x7D, 0x01, 0x7D, 0x7D, 0x7D, 0x7D, 0x01, 0xFF, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0xF8, 0x18, 0x60, 0x80, 0x00, 0x00, 0x00, 0x80,
    0x60, 0x18, 0xF8, 0x00, 0x00, 0x00, 0x20, 0x20, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0,
    0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0xE0, 0x00, 0x00, 0x00, 0x20, 0x20, 0xF8, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x88, 0x68,
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x01, 0x06, 0x18, 0x06, 0x01,
    0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x1F,
    0x20, 0x40, 0x40, 0x40, 0x40, 0x20, 0x1F, 0x00, 0x00, 0x00, 0x40, 0x40, 0x7F, 0x40, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x18, 0x06, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20, 0x20, 0x20, 0xC0, 0x00, 0x00, 0xE0, 0x20, 0x20, 0x20,
    0xE0, 0x00, 0x00, 0x00, 0x40, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x60, 0x20, 0x20, 0x20, 0xE0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xE0, 0x20, 0x20, 0x20, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x20,
    0x20, 0x20, 0xC0, 0x00, 0x00, 0x40, 0x20, 0x20, 0x20, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0A, 0x0A, 0x09, 0x0C, 0x00, 0x00, 0x0F, 0x08, 0x08, 0x08,
    0x0F, 0x00, 0x00, 0x00, 0x08, 0x0F, 0x08, 0x00, 0x00, 0x00, 0x0C, 0x08, 0x09, 0x09, 0x0E, 0x00,
    0x00, 0x0C, 0x00, 0x00, 0x0F, 0x09, 0x09, 0x09, 0x0F, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x0C, 0x0A,
    0x0A, 0x09, 0x0C, 0x00, 0x00, 0x0C, 0x0A, 0x0A, 0x09, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x7F, 0x03, 0x0C, 0x30, 0x0C, 0x03, 0x7F, 0x00, 0x00, 0x38, 0x54, 0x54, 0x58, 0x00, 0x00,
    0x7C, 0x04, 0x04, 0x78, 0x00, 0x00, 0x3C, 0x40, 0x40, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xAA, 0xAA, 0xAA,
    0x28, 0x08, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x03, 0x0C, 0x30, 0x0C, 0x03, 0x7F,
    0x00, 0x00, 0x26, 0x49, 0x49, 0x49, 0x32, 0x00, 0x00, 0x7F, 0x02, 0x04, 0x08, 0x10, 0x7F, 0x00, /***************************************/

};

#endif


