
#include "exynos_4412.h"

/****************MPU6050内部寄存器地址****************/

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x18(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0x68	//MPU6050-I2C地址

/************************延时函数************************/

void mydelay_ms(int time)
{
	int i,j;
	while(time--)
	{
		for(i=0;i<5;i++)
			for(j=0;j<514;j++);
	}
}

/**********************************************************************
 * 函数功能：I2C向特定地址写一个字节
 * 输入参数：
 * 		slave_addr： I2C从机地址
 * 			  addr： 芯片内部特定地址
 * 			  data：写入的数据
**********************************************************************/

void iic_write (unsigned char slave_addr, unsigned char addr, unsigned char data)
{
	/*对时钟源进行512倍预分频  打开IIC中断（每次完成一个字节的收发后中断标志位会自动置位）*/
	I2C5.I2CCON = I2C5.I2CCON | (1<<6) | (1<<5);

	/*设置IIC模式为主机发送模式  使能IIC发送和接收*/
	I2C5.I2CSTAT = 0xd0;
	/*将第一个字节的数据写入发送寄存器  即从机地址和读写位（MPU6050-I2C地址+写位0）*/
	I2C5.I2CDS = slave_addr<<1;
	/*设置IIC模式为主机发送模式  发送起始信号启用总线  使能IIC发送和接收*/
	I2C5.I2CSTAT = 0xf0;

	/*等待从机接受完一个字节后产生应答信号（应答后中断挂起位自动置位）*/
	while(!(I2C5.I2CCON & (1<<4)));

	/*将要发送的第二个字节数据（即MPU6050内部寄存器的地址）写入发送寄存器*/
	I2C5.I2CDS = addr;
	/*清除中断挂起标志位  开始下一个字节的发送*/
	I2C5.I2CCON = I2C5.I2CCON & (~(1<<4));
	/*等待从机接受完一个字节后产生应答信号（应答后中断挂起位自动置位）*/
	while(!(I2C5.I2CCON & (1<<4)));

	/*将要发送的第三个字节数据（即要写入到MPU6050内部指定的寄存器中的数据）写入发送寄存器*/
	I2C5.I2CDS = data;
	/*清除中断挂起标志位  开始下一个字节的发送*/
	I2C5.I2CCON = I2C5.I2CCON & (~(1<<4));
	/*等待从机接受完一个字节后产生应答信号（应答后中断挂起位自动置位）*/
	while(!(I2C5.I2CCON & (1<<4)));

	/*发送停止信号  结束本次通信*/
	I2C5.I2CSTAT = 0xD0;
	/*清除中断挂起标志位*/
	I2C5.I2CCON = I2C5.I2CCON & (~(1<<4));
	/*延时*/
	mydelay_ms(10);
}

/**********************************************************************
 * 函数功能：I2C从特定地址读取1个字节的数据
 * 输入参数：         slave_addr： I2C从机地址
 * 			       addr： 芯片内部特定地址
 * 返回参数： unsigned char： 读取的数值
**********************************************************************/

unsigned char iic_read(unsigned char slave_addr, unsigned char addr)
{

	unsigned char data = 0;

	/*对时钟源进行512倍预分频  打开IIC中断（每次完成一个字节的收发后中断标志位会自动置位）*/
	I2C5.I2CCON = I2C5.I2CCON | (1<<6) | (1<<5);

	/*设置IIC模式为主机发送模式  使能IIC发送和接收*/
	I2C5.I2CSTAT = 0xd0;
	/*将第一个字节的数据写入发送寄存器  即从机地址和读写位（MPU6050-I2C地址+写位0）*/
	I2C5.I2CDS = slave_addr<<1;
	/*设置IIC模式为主机发送模式  发送起始信号启用总线  使能IIC发送和接收*/
	I2C5.I2CSTAT = 0xf0;
	/*等待从机接受完一个字节后产生应答信号（应答后中断挂起位自动置位）*/
	while(!(I2C5.I2CCON & (1<<4)));

	/*将要发送的第二个字节数据（即要读取的MPU6050内部寄存器的地址）写入发送寄存器*/
	I2C5.I2CDS = addr;
	/*清除中断挂起标志位  开始下一个字节的发送*/
	I2C5.I2CCON = I2C5.I2CCON & (~(1<<4));
	/*等待从机接受完一个字节后产生应答信号（应答后中断挂起位自动置位）*/
	while(!(I2C5.I2CCON & (1<<4)));

	/*清除中断挂起标志位  重新开始一次通信  改变数据传送方向*/
	I2C5.I2CCON = I2C5.I2CCON & (~(1<<4));

	/*将第一个字节的数据写入发送寄存器  即从机地址和读写位（MPU6050-I2C地址+读位1）*/
	I2C5.I2CDS = slave_addr << 1 | 0x01;
	/*设置IIC为主机接收模式  发送起始信号  使能IIC收发*/
	I2C5.I2CSTAT = 0xb0;
	/*等待从机接收到数据后应答*/
	while(!(I2C5.I2CCON & (1<<4)));


	/*禁止主机应答信号（即开启非应答  因为只接收一个字节）  清除中断标志位*/
	I2C5.I2CCON = I2C5.I2CCON & (~(1<<7))&(~(1<<4));
	/*等待接收从机发来的数据*/
	while(!(I2C5.I2CCON & (1<<4)));
	/*将从机发来的数据读取*/
	data = I2C5.I2CDS;

	/*直接发起停止信号结束本次通信*/
	I2C5.I2CSTAT = 0x90;
	/*清除中断挂起标志位*/
	I2C5.I2CCON = I2C5.I2CCON & (~(1<<4));
	/*延时等待停止信号稳定*/
	mydelay_ms(10);

	return data;

}


/**********************************************************************
 * 函数功能：MPU6050初始化
**********************************************************************/

void MPU6050_Init ()
{
	iic_write(SlaveAddress, PWR_MGMT_1, 0x00); 		//设置使用内部时钟8M
	iic_write(SlaveAddress, SMPLRT_DIV, 0x07);		//设置陀螺仪采样率
	iic_write(SlaveAddress, CONFIG, 0x06);			//设置数字低通滤波器
	iic_write(SlaveAddress, GYRO_CONFIG, 0x18);		//设置陀螺仪量程+-2000度/s
	iic_write(SlaveAddress, ACCEL_CONFIG, 0x0);		//设置加速度量程+-2g
}



/**********************************************************************
 * 函数功能：主函数
 **********************************************************************/

int main(void)
{

	unsigned char zvalue_h,zvalue_l;						//存储读取结果
	short int zvalue;

	/*设置GPB_2引脚和GPB_3引脚功能为I2C传输引脚*/
	GPB.CON = (GPB.CON & ~(0xF<<12)) | 0x3<<12;			 	//设置GPB_3引脚功能为I2C_5_SCL
	GPB.CON = (GPB.CON & ~(0xF<<8))  | 0x3<<8;				//设置GPB_2引脚功能为I2C_5_SDA

	uart_init(); 											//初始化串口
	MPU6050_Init();											//初始化MPU6050

	printf("\n********** I2C test!! ***********\n");
	while(1)
	{
		zvalue_h = iic_read(SlaveAddress, GYRO_ZOUT_H);		//获取MPU6050-Z轴角速度高字节
		zvalue_l = iic_read(SlaveAddress, GYRO_ZOUT_L);		//获取MPU6050-Z轴角速度低字节
		zvalue  =  (zvalue_h<<8)|zvalue_l;					//获取MPU6050-Z轴角速度

		printf(" GYRO--Z  :Hex: %d	\n", zvalue);			//打印MPU6050-Z轴角速度
		mydelay_ms(100);
	}
	return 0;
}


