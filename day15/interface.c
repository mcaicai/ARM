
#include "exynos_4412.h"

void Delay(unsigned int Time)
{
	while(Time--);
}

int main()
{
	/*设置一级分频*/
	N = WDT.WTCON | (0xFF << 8);
	/*设置二级分频*/
	/*WTCNT递减频率 = PLCK(100000000)/(0xFF + 1)/128 = 3052*/
	WDT.WTCON = WDT.WTCON | (0x3 << 3);
	/*禁止WDT产生中断信号*/
	WDT.WTCON = WDT.WTCON & (~(1 << 2));
	/*使能WDT产生复位信号*/
	WDT.WTCON = WDT.WTCON | 1;
	/*设置计数器的初始值*/
	WDT.WTCNT = (3052 * 5);
	/*使能WDT,计数器开始递减*/
	WDT.WTCON = WDT.WTCON | (1 << 5);

	while(1)
	{
		printf("WDT.WTCNT = %d\n",WDT.WTCNT);
		/*喂狗*/
		WDT.WTCNT = 3052;
		Delay(100000);
	}

	return 0;
}
