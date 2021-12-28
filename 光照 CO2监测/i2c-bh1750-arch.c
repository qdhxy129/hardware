#include "clock.h"          //系统时钟
#include "reg.h"            //寄存器
#include "dev/gpio.h"       //GPIO库
#include "dev/i2c-bh1750-arch.h"  //驱动
#include <stdio.h>
#define DELAY 10
//BH1750函数重封装
#define BH1750_I2C_SDA_1() GPIO_Set(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT)
#define BH1750_I2C_SDA_0() GPIO_Reset(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT)
#define BH1750_I2C_SCL_1() GPIO_Set(BH1750_I2C_SCL_PORT,BH1750_I2C_SCL_BIT)
#define BH1750_I2C_SCL_0() GPIO_Reset(BH1750_I2C_SCL_PORT,BH1750_I2C_SCL_BIT)
#define BH1750_I2C_SDA_READ() GPIO_Get(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT)
//延迟设置
void Delay(unsigned int time)
{
  	clock_delay_usec(time);
}
//ms延时
void Delay_ms(unsigned int Time)
{
  	unsigned char i;
  	while(Time--)
  	{
    		for(i=0;i<100;i++)
    		Delay(10);
  	}
}
//gpio设置
static void
GPIO_Set(int port, int bit)
{
  	GPIO_WRITE_PIN(port, bit, bit);
}
//gpio复位
static void
GPIO_Reset(int port, int bit)
{
  	GPIO_WRITE_PIN(port, bit, 0);
}
//gpio读 暂时未用
static int
GPIO_Get(int port, int bit)
{
  	return GPIO_READ_PIN(port, bit);
}
//BH1750接口配置
void BH1750_Config(void)
{  
	 //设置成输出
 	GPIO_SET_OUTPUT(BH1750_I2C_SCL_PORT,BH1750_I2C_SCL_BIT);
 	GPIO_SET_OUTPUT(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT);
}
void I2C_Start(void)
{
	BH1750_I2C_SDA_1();
	BH1750_I2C_SCL_1();
	Delay(10);
	BH1750_I2C_SDA_0();
	Delay(10);
	BH1750_I2C_SCL_0();
	Delay(10);
}
void I2C_Stop(void)
{
	BH1750_I2C_SDA_0();
	BH1750_I2C_SCL_1();
	Delay(10);
	BH1750_I2C_SDA_1();
}
void I2C_SendByte(unsigned char _ucByte)
{
	unsigned char i;
	//先发送字节的高位bit7
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			BH1750_I2C_SDA_1();
		}
		else
		{
			BH1750_I2C_SDA_0();
		}
		Delay(10);
		BH1750_I2C_SCL_1();
		Delay(10);	
		BH1750_I2C_SCL_0();
		if (i == 7)
		{
			 BH1750_I2C_SDA_1();//释放总线
		}
		_ucByte <<= 1;//左移一个bit
		Delay(10);
	}
}
unsigned char I2C_ReadByte(void)
{
	unsigned char i;
	unsigned char value;
	//读到第1个bit为数据的bit7
	//转换为输入
	GPIO_SET_INPUT(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT);
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		BH1750_I2C_SCL_1();
		Delay(10);
		if (BH1750_I2C_SDA_READ())
		{
			value++;
		}
		BH1750_I2C_SCL_0();
		Delay(10);
	}
	GPIO_SET_OUTPUT(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT);
	BH1750_I2C_SDA_1();	/* CPU释放SDA总线 */
	return value;
}
unsigned char I2C_WaitAck(void)
{
	unsigned char re;
	BH1750_I2C_SDA_1();	/* CPU释放SDA总线 */
	Delay(10);
	BH1750_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	Delay(10);
	//转换输入
	GPIO_SET_INPUT(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT);
	if (BH1750_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
		re = 1;
	else
		re = 0;
	BH1750_I2C_SCL_0();
	GPIO_SET_OUTPUT(BH1750_I2C_SDA_PORT,BH1750_I2C_SDA_BIT);
	BH1750_I2C_SDA_1();	/* CPU释放SDA总线 */
	Delay(10);
	return re;
}
void I2C_Ack(void)
{
	BH1750_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	Delay(10);
	BH1750_I2C_SCL_1();	/* CPU产生1个时钟 */
	Delay(10);
	BH1750_I2C_SCL_0();
	Delay(10);
	BH1750_I2C_SDA_1();	/* CPU释放SDA总线 */
}
void I2C_NAck(void)
{
	BH1750_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	Delay(10);
	BH1750_I2C_SCL_1();	/* CPU产生1个时钟 */
	Delay(10);
	BH1750_I2C_SCL_0();
	Delay(10);	
}
//相当于IIC初始化
unsigned char I2C_CheckDevice(unsigned char _Address)
{	
	unsigned char ucAck;
	//配置GPIO
	BH1750_Config();
	//给一个停止信号, 复位I2C总线上的所有设备到待机模式
	I2C_Stop();
	I2C_Start();		//发送启动信号
	//发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传
	I2C_SendByte(_Address | BH1750_I2C_WR);
	ucAck = I2C_WaitAck();	// 检测设备的ACK应答
	I2C_Stop();		// 发送停止信号 
	return ucAck;
}
unsigned char BH1750_Byte_Write(unsigned char data)
{
	I2C_Start();
	//发送写地址
	I2C_SendByte(BH1750_Addr|0);
	if(I2C_WaitAck()==1)
	return 1;
	//发送控制命令
	I2C_SendByte(data);
	if(I2C_WaitAck()==1)
	return 2;
	I2C_Stop();
	return 0;
}
unsigned int BH1750_Read_Measure(void)
{
	unsigned int receive_data=0; 
	I2C_Start();
	//发送读地址
	I2C_SendByte(BH1750_Addr|1);
	if(I2C_WaitAck()==1)
	return 0;
	//读取高八位
	receive_data=I2C_ReadByte();
	I2C_Ack();
	//读取低八位
	receive_data=(receive_data<<8)+I2C_ReadByte();
	I2C_NAck();
	I2C_Stop();
	return receive_data;	//返回读取到的数据
}
//BH1750s上电
void BH1750_PowerON(void)
{
	BH1750_Byte_Write(POWER_ON);
}
//BH1750s断电
void BH1750_PowerOFF(void)
{
	BH1750_Byte_Write(POWER_OFF);
}
//BH1750复位,仅在上电时有效
void BH1750_RESET(void)
{
	BH1750_Byte_Write(MODULE_RESET);
}
//BH1750初始化
void BH1750_Init(void)
{
	BH1750_Power_ON();//BH1750s上电
	BH1750_Byte_Write(Measure_Mode);
	Delay_ms(120);
	//检测传感器是否在线
  	if(i2c_CheckDevice(BH1750_Addr)==0)
 	{
    		printf("BH1750 OK");
  	}
  	else
  	{
    		printf("BH1750 LOSE");
  	}
}
void LightGetData(void)
{
	lx=BH1750_Read_Measure();
	lx=lx*6/5;
}