#include <string.h>
#include "IR.h"

void init_IR()
{
    //接收
    EA  = 1;  //总中断开
    EX0 = 1;  //IR 接收头使用外部中断0 来处理
    IT0 = 1;  //下降沿触发

    //发射
    TMOD |= 0x01; //T0 16位工作方式
    LaunchLED = 1;      //发射端口常态为高电平
}

//发射
void IR_launch(IR_CODE ir_code)
{
    IR_launch_time(1, m9);   //高电平9mS
    IR_launch_time(0, m4_5); //低电平4.5mS

    /*┈ 发送4帧数据┈*/
    IR_launch_frame(ir_code.custom_height);
    IR_launch_frame(ir_code.custom_lower);
    IR_launch_frame(ir_code.ir_code);
    IR_launch_frame(ir_code.re_ir_code);

    /*┈┈ 结束码 ┈┈*/
    IR_launch_time(1, m_65);
    IR_launch_time(0, m40);
}

//发送 1 帧数据
void IR_launch_frame(unsigned char frame)
{
    char i = 0;
    for(i=0; i<8; i++)  //循环8次移位
    {
        IR_launch_time(1, m_65);  //高电平0.65ms
        if(frame >>i & 0x1)
            IR_launch_time(0, m1_6); //发送最低位
        else
            IR_launch_time(0, m_56);
     }
}

//38KHz脉冲发射 + 延时程序
void IR_launch_time(bit status,unsigned int t)
{
  TH0 = t>>8; //输入T0初始值
  TL0 = t;
  TF0 = 0;    //清0
  TR0 = 1;    //启动定时器0
  if(0 == status)
  {
    //BT=0时不发射38KHz脉冲只延时；BT=1发射38KHz脉冲且延时；
    while(! TF0);
  }
  else
  {
      while(1)
      {
          /**
           * 38KHz脉冲，占空比5:26
           * 以下是逻辑分析仪测试结果
           * 3:23us  识别正常
           * 6:23us  识别正常
           * 10:23us 识别失败
           * 12:23us 识别失败
           * 16:23us 识别失败
           */
          LaunchLED = 0;
          if(TF0)break;
          if(TF0)break;
          LaunchLED = 1;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
          if(TF0)break;
         }
    }
    TR0=0; //关闭定时器0
    TF0=0; //标志位溢出则清0
    LaunchLED =1; //脉冲停止后，发射端口常态为高电平
}

//接收
IR_CODE IR_recv()
{
    /**
     * 数据格式：
     * 9ms低电平 4.5ms高电平 头部
     * 定制高位  定制低位  数据码  数据反码
     * 1: 560us低电平 1680us高电平  0.56ms 1.7ms
     * 0: 560us低电平 560us高电平   0.56ms 0.56ms
     */
    IR_CODE ir_code;
    unsigned char i,k;
    unsigned char *ir_char_p;
    unsigned char ir_char;
    ir_char_p = (unsigned char *)&ir_code;

    //栈分配 IR_CODE 竟然还要手动清0
    memset(&ir_code, 0, 4);

    delayms(5); //9ms 内必须是低电平否则就不是头信息
    if(0 == IR)
    {
        while(! IR);//等待4.5ms的高电平

        //检测是否是 2.5ms 重码
        delayms(3);
        if(1 == IR)
        {
            //k 4位编码
            for(k=0; k<4; k++)
            {
                ir_char = 0x0;
                //i 每一个编码的 8bit
                for(i=0;i<8;i++)
                {
                    while(IR);     //等待变为低电平时
                    while(! IR);   //等待变为高电平后
                    delay700us();  //休眠700us 后读值
                    ir_char |= (char)IR << i;//先存低位
                    //使用下面指针操作就会失败出现不稳定
                    //*ir_char_p |= (char)IR << i;
                }
                *ir_char_p = ir_char;
                ir_char_p++;
            }

            //计算反码 code码是否正确
            if(ir_code.ir_code != ~(ir_code.re_ir_code))
            {
                memset(&ir_code, 0, 4);
            }
        }
    }
    return ir_code;
}