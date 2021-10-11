#include <reg52.h> //C:\Keil_v5\C51\Inc\reg52.h
//#include "regx51.h"
#include "delay.h"
#include <intrins.h>
#include <stdio.h>

sbit LED = P0^0;
 
#if 0
void Delay(void);

void Delay(void)
{
	int i, j;
	for(i=0;i<10;i++)
	{
		for(j=0;j<10000;j++)
		{
		}
	}
}
#endif

void main(){
	
	while(1)
	{
		LED = 0;
		//Delay();
		delayms(1000);
		LED = 1;
		delayms(100);
		LED = 0;
		delayms(100);
		LED = 1;
		delayms(1000);		
		LED = 0;
		//Delay();
		delayms(1000);
		LED = 1;
		delayms(1000);	
	}
}
