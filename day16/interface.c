#include "exynos_4412.h"

void Delay(unsigned int Time)
{
	while(Time--);
}

//IRQ异常处理
void do_irq(void)
{
	unsigned int IrqNum = 0;
	/*从中断控制器中获取当前中断的中断号*/
	IrqNum = CPU0.ICCIAR & 0x3FF;
	
	/*根据中断号处理不同的中断*/
	switch(IrqNum)
	{
		case 0:
			//0号中断的处理程序
			break;
		case 1:
			//1号中断的处理程序
			break;
			/*
			 * ... ...
			 */
		case 57:
			printf("Key2 Pressed\n");
			/*清除GPIO控制器中GPX1_1的中断挂起标志位*/
			EXT_INT41_PEND = (1 << 1);
			/*将当前中断的中断号写回到中断控制器中，以这种方式来告知中断控制器当前的中断已经处理完成，可以发送其它中断*/
			CPU0.ICCEOIR = CPU0.ICCEOIR & (~(0x3FF)) | (57);
			break;
			/*
			 * ... ...
			 */
		case 159:
			//159号中断的处理程序
			break;
		default:
			break;
	}

}

int main()
{
	/*外设层次 - 让外部的硬件控制器产生一个中断信号发送给中断控制器*/
	/*将GPX1_1设置成中断功能*/
	GPX1.CON = GPX1.CON | (0xF << 4);
	/*设置GPX1_1的中断触发方式为下降沿触发*/
	EXT_INT41_CON = EXT_INT41_CON & (~(0x7 << 4)) | (0x2 << 4);
	/*使能GPX1_1的中断功能*/
	EXT_INT41_MASK = EXT_INT41_MASK & (~(1 << 1));

	/*中断控制器层次 - 让中断控制器接收外设产生的中断信号并对其进行管理然后再转发给CPU处理*/
	/*全局使能中断控制器使其能接收外设产生的中断信号并转发到CPU接口*/
	ICDDCR = ICDDCR | 1;
	/*在中断控制器中使能57号中断，使中断控制器接收到57号中断后能将其转发到CPU接口*/
	ICDISER.ICDISER1 = ICDISER.ICDISER1 | (1 << 25);
	/*选择由CPU0来处理57号中断*/
	ICDIPTR.ICDIPTR14 = ICDIPTR.ICDIPTR14 & (~(0xFF << 8)) | (0X01 << 8);
	/*使能中断控制器和CPU0之间的接口，使中断控制器转发的中断信号能够到达CPU0*/
	CPU0.ICCICR = CPU0.ICCICR | 1;

	GPX2.CON = GPX2.CON & (~(0xF << 28)) | (0x1 << 28);

    while(1)
	{   
		/*点亮LED2*/
		GPX2.DAT = GPX2.DAT | (1 << 7); 
		/*延时*/
		Delay(1000000);
		/*熄灭LED2*/
		GPX2.DAT = GPX2.DAT & (~(1 << 7));
		/*延时*/
		Delay(1000000);
	}   

	return 0;
}
