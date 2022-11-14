
#include "exynos_4412.h"

int main()
{
	unsigned int AdcValue;

	/*设置ADC精度为12bit*/
	ADCCON = ADCCON | (1 << 16);
	/*使能ADC分频器*/
	ADCCON = ADCCON | (1 << 14);
	/*设置ADC分频值 ADC时钟频率=PLCK/(19+1)=5MHZ ADC转换频率=5MHZ/5=1MHZ*/
	ADCCON = ADCCON & (~(0xFF << 6)) | (19 << 6);
	/*关闭待机模式，使能正常模式*/
	ADCCON = ADCCON & (~(1 << 2));
	/*关闭通过读使能AD转换*/
	ADCCON = ADCCON & (~(1 << 1));
	/*选择转换通道，3通道*/
	ADCMUX = 3;

	while(1)
	{
		/*开始转换*/
		ADCCON = ADCCON | 1;
		/*等待转换完成*/
		while(!(ADCCON & (1 << 15)));
		/*读取转换结果*/
		AdcValue = ADCDAT & 0xFFF;
		/*将结果转换成实际的电压值mv*/
		AdcValue = AdcValue * 0.44;
		/*打印转换结果*/
		printf("AdcValue = %dmv\n",AdcValue);

	}
	return 0;
}
