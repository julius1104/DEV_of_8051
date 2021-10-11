#include "eeprom.h"

/* 执行完操作以后安全锁 */
void lock_ISP()
{
    ISP_CONTR = 0;
    ISP_CMD = 0;
    ISP_TRIG = 0;
    ISP_ADDRH = 0xff;
    ISP_ADDRL = 0xff;
}

/* 擦除指定地址所在的整个扇区  */
void erase(unsigned int addr)
{
    addr += BASE_ADDR;

    //发送地址
    ISP_ADDRH = addr >> 8;
    ISP_ADDRL = addr;

    //发送解锁命令
    ISP_CONTR = En_Wait_ISP;

    //发擦除命令
    ISP_CMD = CMD_Erase;

    //发送触发命令
    ISP_TRIG = 0x46;
    ISP_TRIG = 0xB9;

    //最后锁定 ISP 仿误操作
    lock_ISP();
}

unsigned char read(unsigned int addr)
{
    addr += BASE_ADDR;

    //发送地址
    ISP_ADDRH = addr >> 8;
    ISP_ADDRL = addr;

    //发送解锁命令
    ISP_CONTR = En_Wait_ISP;

    //发读命令
    ISP_CMD = CMD_Read;

    //发送触发命令
    ISP_TRIG = 0x46;
    ISP_TRIG = 0xB9;

    //最后锁定 ISP 仿误操作
    lock_ISP();

    return ISP_DATA;
}

void prog(unsigned int addr, unsigned char dat)
{
    addr += BASE_ADDR;

    //发送要保存的数据
    ISP_DATA = dat;

    //发送地址
    ISP_ADDRH = addr >> 8;
    ISP_ADDRL = addr;

    //发送解锁命令
    ISP_CONTR = En_Wait_ISP;

    //发编程命令
    ISP_CMD = CMD_Prog;

    //发送触发命令
    ISP_TRIG = 0x46;
    ISP_TRIG = 0xB9;

    //最后锁定 ISP 仿误操作
    lock_ISP();
}