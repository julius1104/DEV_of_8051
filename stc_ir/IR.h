#include <reg52.h>
#include "delay.h"
#ifndef __IR__
#define __IR__
//公用
typedef struct {
    unsigned char custom_height;
    unsigned char custom_lower;
    unsigned char ir_code;
    unsigned char re_ir_code;
} IR_CODE, *pIR_CODE;

//接收
sbit IR     = P3 ^ 2;//红外线一体接收头 OUT

void init_IR();
IR_CODE IR_recv();

//发射延时
#define m9   (65536-9000)        //约9mS
#define m4_5 (65536-4500)        //约4.5mS
#define m1_6 (65536-1630)        //约1.65mS
#define m_65 (65536-580)         //约0.65mS
#define m_56 (65536-560)         //约0.56mS
#define m40  (65536-40000)       //约40mS
#define m56  (65536-56000)       //56mS
#define m2_25 (65536-2250)        //约2.25mS

sbit LaunchLED = P0 ^ 4;//红外线发射LED 接PNP三极管基极, 经过试验不使用三极管时有效距离是40厘米, 可见 IO 下拉电流很小
void IR_launch(IR_CODE);
void IR_launch_time(bit,unsigned int);
void IR_launch_frame(unsigned char);
#endif