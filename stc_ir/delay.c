#include <intrins.h>
#include "delay.h"
void delayms(int ms) //@11.0592MHz
{
    unsigned char i, j;
    while(ms--)
    {
        _nop_();
        i = 2;
        j = 199;
        do
        {
            while (--j);
        }
        while (--i);
    }
}

void delay700us() //@11.0592MHz
{
    unsigned char i, j;
    _nop_();
    i = 2;
    j = 61;
    do
    {
        while (--j);
    } while (--i);
}