
#include "exynos_4412.h"

int main()
{
	unsigned int OldSec = 0, NewSec = 0;

	/*使能RTC控制*/
	RTCCON = RTCCON | 1;
	/*校准时间信息*/
	RTC.BCDYEAR = 0x023;
	RTC.BCDMON  = 0x12;
	RTC.BCDDAY  = 0x7;
	RTC.BCDWEEK = 0x31;
	RTC.BCDHOUR = 0x23;
	RTC.BCDMIN  = 0x59;
	RTC.BCDSEC  = 0x50;
	/*禁止RTC控制*/
	RTCCON = RTCCON &  (~(1));

	while(1)
	{
		NewSec = RTC.BCDSEC;
		if(OldSec != NewSec)
		{
			printf("20%x-%x-%x %x %x:%x:%x\n",RTC.BCDYEAR, RTC.BCDMON, RTC.BCDWEEK, RTC.BCDDAY, RTC.BCDHOUR, RTC.BCDMIN, RTC.BCDSEC);	
			OldSec = NewSec;
		}
	}
	return 0;
}
