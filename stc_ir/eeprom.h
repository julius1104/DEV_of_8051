#include <reg52.h>
#ifndef __EEPROM__
#define __EEPROM__
/**
 * STC90C52 结尾是 90C
 * EEPROM 5K
 * SRAM 215字节
 * 每个扇区512字节  5K / 512 = 10 个扇区
 * 扇区首地址 2000h 结束地址 33ffh
 */

/* FLASH 首地址 */
#define BASE_ADDR 0x2000

/* 特殊功能寄存器声明 */
sfr ISP_DATA  = 0xe2;
sfr ISP_ADDRH = 0xe3;
sfr ISP_ADDRL = 0xe4;
sfr ISP_CMD   = 0xe5;
sfr ISP_TRIG  = 0xe6;
sfr ISP_CONTR = 0xe7;

/* 定义命令字节 */
#define CMD_Read    0x01 //字节读数据命令
#define CMD_Prog    0x02 //字节编程数据命令
#define CMD_Erase   0x03 //扇区擦除数据命令
#define En_Wait_ISP 1<<7 | 1<<1  //设置等待时间 ,并使能ISP/IAP 11.0592 晶振

void lock_ISP();
void erase(unsigned int);
unsigned char read(unsigned int);
void prog(unsigned int, unsigned char);

#endif