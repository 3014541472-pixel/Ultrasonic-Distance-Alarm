#include <REGX52.H>
#include "LCD1602.h"
#include "Delay.h"
#include "UT.h"

int Result = 0;

sbit Buzzer = P3^7;

unsigned char PWM_Duty = 30;
unsigned char PWM_Count = 0;



void Buzzer_SetDuty(unsigned char Duty)
{
	if(Duty > 100)
	{
		Duty = 100;
	}

	PWM_Duty = Duty;
}



/**
  * @brief 根据距离显示风险等级
  * @param Distance 距离，单位cm
  */
void Risk_Display(int Distance)
{
	if(Distance <= 10)
	{
		/* 高风险 */
		LCD_ShowChar(2,1,'H');
		Buzzer_SetDuty(100);
	}
	else if(Distance <= 20)
	{
		/* 中风险 */
		LCD_ShowChar(2,1,'M');
		Buzzer_SetDuty(50);
	}
	else if(Distance <= 30)
	{
		/* 低风险 */
		LCD_ShowChar(2,1,'L');
		Buzzer_SetDuty(25);
	}
	else
	{
		/* 无风险 */
		LCD_ShowChar(2,1,'N');
		Buzzer_SetDuty(0);
	}
}





void Timer1_Init(void)
{
	TMOD &= 0x0F;
	TMOD |= 0x10;		// 定时器1，方式1，16位定时器

	TH1 = 0xFF;
	TL1 = 0x9C;		// 65536-100=65436=0xFF9C

	TF1 = 0;
	ET1 = 1;
	EA = 1;
	TR1 = 1;
}



void main(void)
{
	LCD_Init();
	UT_Init();

	/* 显示距离单位 */
	LCD_ShowChar(1,4,'c');
	LCD_ShowChar(1,5,'m');
	
	Buzzer = 1;		// 高电平关闭蜂鸣器

	Timer1_Init();
	
	while(1)
	{
		/* 触发一次超声波测距 */
		UT_triggr();
		
		/* 获取距离，单位cm */
		Result = measure();

		/* 第一行显示距离 */
		LCD_ShowNum(1,1,Result,3);
	
		/* 第二行显示风险等级 */
		Risk_Display(Result);

		Delay(60);
	}
}





void Timer1_Routine(void) interrupt 3
{
	TH1 = 0xFF;
	TL1 = 0x9C;

	PWM_Count++;

	if(PWM_Count >= 100)
	{
		PWM_Count = 0;
	}

	/*
	 * 有源蜂鸣器低电平触发
	 */
	if(PWM_Count < PWM_Duty)
	{
		Buzzer = 0;
	}
	else
	{
		Buzzer = 1;
	}
}