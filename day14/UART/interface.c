
#include "exynos_4412.h"

void UART_Init(void)
{
	/*1.将GPA1_0和GPA1_1设置成UART2的接收和发送引脚 GPA1CON[7:0]*/
	GPA1.CON = GPA1.CON & (~(0xFF << 0)) | (0x22 << 0);
	/*2.设置UART2的帧格式 8位数据位 1位停止位 无校验 正常模式 ULCON2[6:0]*/
	UART2.ULCON2 = UART2.ULCON2 & (~(0x7F << 0)) | (0x3 << 0);
	/*3.设置UART2的接收和发送模式为轮询模式 UCON2[3:0]*/
	UART2.UCON2 = UART2.UCON2 & (~(0xF << 0)) | (0x5 << 0);
	/*4.设置UART2的波特率为115200 UBRDIV2/UFRACVAL2*/
	UART2.UBRDIV2 = 53;
	UART2.UFRACVAL2 = 4;
}

void UART_Send_Byte(char Dat)
{
	/*等待发送寄存器为空，即上一个数据已经发送完成 UTRSTAT2[1]*/
	while(!(UART2.UTRSTAT2 & (1 << 1)));
	/*将要发送的数据写入发送寄存器 UTXH2*/
	UART2.UTXH2 = Dat;
}

char UART_Rec_Byte(void)
{
	char Dat = 0;
	/*判断接收寄存器是否接收到了数据 UTRSTAT2[0]*/
	if(UART2.UTRSTAT2 & 1)
	{
		/*从接收寄存器中读取接收到的数据 URXH2*/
		Dat = UART2.URXH2;
		return Dat;
	}
	else
	{
		return 0;
	}
}

void UART_Send_Str(char * pstr)
{
	while(*pstr != '\0')
		UART_Send_Byte(*pstr++);
}

int main()
{
	char RecDat = 0;
	UART_Init();

	while(1)
	{
		/*
		RecDat = UART_Rec_Byte();
		if(RecDat == 0)
		{
			
		}
		else
		{
			RecDat = RecDat + 1;
			UART_Send_Byte(RecDat);
		}
		*/
		
		/*
		UART_Send_Str("Hello World\n");
		*/

		printf("Hello World\n");
	}
	return 0;
}
