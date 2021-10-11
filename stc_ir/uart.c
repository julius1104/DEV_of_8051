#include <reg52.h>
#include "uart.h"
void init_uart()
{
    //定时器1 溢出决定波特率
    EA  = 1;      //总中断开x`
    TMOD |= 1<<5; //定时器1 自动重装模式
    TH1 = 0xfd;   //当TL1中溢出时 TH1 的值自动重装进去
    TL1 = 0xfd;   //省去一个中断处理函数
    TR1 = 1;      //开始计数
	
		//Set SCON ; 10-bit UART, 8-bit Data, variable baud rate
    SM0 = 0;
    SM1 = 1;
		REN = 1;
		// above baud rate is 9600
}

void send_str(unsigned char *str)
{
    while(*str)
    {
        SBUF = *str;
        while(! TI);
        TI = 0;
        str++;
    }
}

void send_hex(unsigned char hex)
{
    SBUF = hex;
    while(! TI);
    TI = 0;
}

void send_code(IR_CODE ir_code)
{
    unsigned char c;
    unsigned char *p;
    int i,j;
    p = (unsigned char *)&ir_code;
    send_str("custom:0x");
    for(i=0; i<4; i++)
    {
        if(2 == i)
        {
            send_str(" code:0x");
        }
        for(j=1; j>=0; j--)
        {
            c = (*p>>(4*(j))) & 0xf;
            if(0<=c && c<=9)
            {
                send_hex('0' + c);
            }
            else
            {
                send_hex('A' + c - 0xa);
            }
        }
        p++;
    }
    send_str("\r\n");
}