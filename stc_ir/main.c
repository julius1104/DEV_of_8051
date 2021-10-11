#include <reg52.h>
#include <intrins.h>
#include <string.h>
#include "uart.h"
#include "delay.h"
#include "IR.h"
#include "eeprom.h"

IR_CODE read_ir_code(unsigned char);
void save_block_ir_code();
void save_ir_code(IR_CODE ir_code,unsigned int addr);

//全局接收红外线信号存放
IR_CODE global_ir_code;

//从eeprom 中读取 按键值
IR_CODE k3_ir_code;
IR_CODE k4_ir_code;
IR_CODE k5_ir_code;
IR_CODE k6_ir_code;

//3个按键
sbit K3 = P3 ^ 5;
sbit K4 = P3 ^ 4;
sbit K5 = P3 ^ 3;

#ifdef DEBUG
//main 中 EX1 = 1;
//外部中断1 按下时发送红外线信号
void infrared_led_int1() interrupt 2
{
    EX1 = 0;
    IR_launch(global_ir_code);
    EX1 = 1;
}
#endif

void main()
{
		IR_CODE tt;
    //总中断开关
    EA  = 1;
    init_uart();
    init_IR();

    //全局接收红外线清0
    memset(&global_ir_code, 0, 4);

    //从eeprom 中读取 按键值
    k3_ir_code = read_ir_code(0);
    k4_ir_code = read_ir_code(4);
    k5_ir_code = read_ir_code(8);

    while(1)
    {
				memset(&tt, 0xff, 4);
        send_code(tt);
        //按键按下
        if(0 == K3)
        {
            //去抖动后
            delayms(30);
            if(0 == K3)
            {
                //关中断 中断打开时，时钟可能不准
                EA  = 0;

                //如果有全局接收 则存入 eeprom
                if(global_ir_code.ir_code)
                {
                    k3_ir_code = global_ir_code;
                    save_block_ir_code();
                    memset(&global_ir_code, 0, 4);

                    //闪灯2次表示 存入成功
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                    delayms(300);
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                }
                else
                {
                    IR_launch(k3_ir_code);

                    //闪灯1次表示 发射成功
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                }
                //开中断
                EA  = 1;
            }
        }
        //按键按下
        if(0 == K4)
        {
            //去抖动后
            delayms(30);
            if(0 == K4)
            {
                //关中断 中断打开时，时钟可能不准
                EA  = 0;

                //如果有全局接收 则存入 eeprom
                if(global_ir_code.ir_code)
                {
                    k4_ir_code = global_ir_code;
                    save_block_ir_code();
                    memset(&global_ir_code, 0, 4);

                    //闪灯2次表示 存入成功
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                    delayms(300);
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                }
                else
                {
                    IR_launch(k4_ir_code);

                    //闪灯1次表示 发射成功
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                }
                //开中断
                EA  = 1;
            }
        }
        //按键按下
        if(0 == K5)
        {
            //去抖动后
            delayms(30);
            if(0 == K5)
            {
                //关中断 中断打开时，时钟可能不准
                EA  = 0;

                //如果有全局接收 则存入 eeprom
                if(global_ir_code.ir_code)
                {
                    k5_ir_code = global_ir_code;
                    save_block_ir_code();
                    memset(&global_ir_code, 0, 4);

                    //闪灯2次表示 存入成功
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                    delayms(300);
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                }
                else
                {
                    IR_launch(k5_ir_code);

                    //闪灯1次表示 发射成功
                    P1 = 0x0;
                    delayms(300);
                    P1 = 0xff;
                }
                //开中断
                EA  = 1;
            }
        }
    }
}

void IR_int() interrupt 0
{
    IR_CODE ir_code;
    EX0 = 0;//处理过程中 关中断
    ir_code = IR_recv();
    if(ir_code.ir_code)
    {
        //点亮P1 显示收到了编码
        P1 = 0x0;
        delayms(300);
        P1 = 0xff;

        //发送到串口
        send_code(ir_code);

        //给全局编码赋值
        global_ir_code = ir_code;
    }
    EX0 = 1;//处理结束后 开中断
}

//读取 eeprom 中的值
IR_CODE read_ir_code(unsigned char addr)
{
    IR_CODE ir_code;
    ir_code.custom_height = read(addr);
    ir_code.custom_lower  = read(addr+1);
    ir_code.ir_code       = read(addr+2);
    ir_code.re_ir_code    = read(addr+3);
    return ir_code;
}

//保存所有编码
void save_block_ir_code()
{
    erase();
    save_ir_code(k3_ir_code, 0);
    save_ir_code(k4_ir_code, 4);
    save_ir_code(k5_ir_code, 8);
}

//保存一个编码
void save_ir_code(IR_CODE ir_code,unsigned int addr)
{
    prog(addr,   ir_code.custom_height);
    prog(addr+1, ir_code.custom_lower);
    prog(addr+2, ir_code.ir_code);
    prog(addr+3, ir_code.re_ir_code);
}