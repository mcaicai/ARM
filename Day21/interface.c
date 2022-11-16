
#include "exynos_4412.h"

void Delay(unsigned int Time)
{
	while(Time --);
}

int main()
{
	/*1.将GPD0_0引脚设置成PWM0的输出引脚*/
	GPD0.CON = GPD0.CON & (~(0xF)) | (0x2);
	/*2.设置PWM0的一级分频	一级分频倍数设置为100倍*/
	PWM.TCFG0 = PWM.TCFG0 & (~(0xFF)) | 99;
	/*2.设置PWM0的二级分频	二级分频倍数设置为1倍  递减计数器递减频率 = PLCK / (99 + 1) / 1 = 1M*/
	PWM.TCFG1 = PWM.TCFG1 & (~(0xF));
	/*4.设置PWM0为自动重装载，使其能够产生连续的脉冲信号*/
	PWM.TCON = PWM.TCON | (1 << 3);
	/*5.设置PWM0的频率为500HZ*/
	PWM.TCNTB0 = 2000;
	/*6.设置PWM0的占空比为50%*/
	PWM.TCMPB0 = 1000;
	/*7.将TCNTB0中的值手动装载到递减计数器*/
	PWM.TCON = PWM.TCON | (1 << 1);
	/*8.关闭手动更新*/
	PWM.TCON = PWM.TCON & (~(1 << 1));
	/*9.使能PWM0，递减计数器开始递减*/
	PWM.TCON = PWM.TCON | 1;

	while(1)
	{
		PWM.TCON = PWM.TCON | 1;
		Delay(1000000);
		PWM.TCON = PWM.TCON & (~(1));
		Delay(1000000);		
	}
	return 0;
}
