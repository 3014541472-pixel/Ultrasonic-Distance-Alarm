#include <REGX52.H>
#include "delay.h"
sbit Trig=P1^6;//定义超声波模块引脚
sbit Echo=P1^5;
/**
   *@breaf 定时计数器1初始化
   *@param  无
   *@retval  无
   */
void T0_Init()
{
    TMOD=0x01;//定义计时器0工作方式为1
    TH0=0;//定义定时器初值
    TL0=0;
    ET0=1;//开定时器0中断
    EA=1;//开总中断
}
/**
  *@beaf 超声波模块初始化
  *@param 无
  *@retval 无
*/
void UT_Init()
{
    Echo=0;
    Trig=0;
    T0_Init();
}
/**
  *@beaf 超声波模块触发函数
  *@param 无
  *@retval 无
*/
void UT_triggr()
{
    Trig=1;
    Delay10us(2);
    Trig=0;    
}
/**
  *@beaf超声波测距
  *@param 无
  *@retval distance 测试的距离
*/
unsigned int measure()
{
    unsigned int a,distance;
    while(Echo==0);
        TR0=1;
    while(Echo);
        TR0=0;
    a=TH0*256+TL0;
       TH0=0;
       TL0=0;
    distance= a*1.7/100;
    return distance;        
}
 