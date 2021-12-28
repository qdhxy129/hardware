#include "clock.h"	//系统时钟
#include "reg.h"	//寄存器
#include "dev/gpio.h"	//GPIO库
#include "dev/i2c-CCS811-arch.h"	//驱动
#include "dev/i2c-bh1750-arch.h"	//驱动
#include <stdio.h>
unsigned char BUF[12];
unsigned char Information[10];
unsigned char MeasureMode, Status, Error_ID;
unsigned char FlagGetId = 1;
unsigned char n = 4;	// 3次读取ID都对则说明没问题
unsigned char temp = 0x5a;
#define DELAY 10
//CCS811函数重封装
#define CCS811_I2C_SDA_1() GPIO_Set(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT)
#define CCS811_I2C_SDA_0() GPIO_Reset(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT)
#define CCS811_I2C_SCL_1() GPIO_Set(CCS811_I2C_SCL_PORT,CCS811_I2C_SCL_BIT)
#define CCS811_I2C_SCL_0() GPIO_Reset(CCS811_I2C_SCL_PORT,CCS811_I2C_SCL_BIT)
#define CCS811_I2C_SDA_READ() GPIO_Get(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT)
#define CCS811_CS_ON()      GPIO_Reset(CCS811_aWAKE_PORT,CCS811_aWAKE_BIT)
#define CCS811_CS_OFF()     GPIO_Set(CCS811_aWAKE_PORT,CCS811_aWAKE_BIT)
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
//CCS811接口配置
void CCS811_Config(void)
{  
	//设置成输出
 	GPIO_SET_OUTPUT(CCS811_I2C_SCL_PORT,CCS811_I2C_SCL_BIT);
 	GPIO_SET_OUTPUT(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT);
 	GPIO_SET_OUTPUT(CCS811_aWAKE_PORT,CCS811_aWAKE_BIT);
}
void I2C_CCS811_Start(void)
{
	CCS811_I2C_SDA_1();
	CCS811_I2C_SCL_1();
	Delay(10);
	CCS811_I2C_SDA_0();
	Delay(10);
	CCS811_I2C_SCL_0();
	Delay(10);
}
void I2C_CCS811_Stop(void)
{
	CCS811_I2C_SDA_0();
	CCS811_I2C_SCL_1();
	Delay(10);
	CCS811_I2C_SDA_1();
}
void I2C_CCS811_SendByte(unsigned char cucByte)
{
	unsigned char i;
	//先发送字节的高位bit7
	for (i = 0; i < 8; i++)
	{		
		if (cucByte & 0x80)
		{
			CCS811_I2C_SDA_1();
		}
		else
		{
			CCS811_I2C_SDA_0();
		}
		Delay(10);
		CCS811_I2C_SCL_1();
		Delay(10);	
		CCS811_I2C_SCL_0();
		if (i == 7)
		{
			 CCS811_I2C_SDA_1();	//释放总线
		}
		cucByte <<= 1;	//左移一个bit
		Delay(10);
	}
}
unsigned char I2C_CCS811_ReadByte(void)
{
	unsigned char i;
	unsigned char value;
	//读到第1个bit为数据的bit7
	//转换为输入
	GPIO_SET_INPUT(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT);
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		CCS811_I2C_SCL_1();
		Delay(10);
		if (CCS811_I2C_SDA_READ())
		{
			value++;
		}
		CCS811_I2C_SCL_0();
		Delay(10);
	}
	GPIO_SET_OUTPUT(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT);
	CCS811_I2C_SDA_1();	//CPU释放SDA总线
	return value;
}
unsigned char I2C_CCS811_WaitAck(void)
{
	unsigned char re;
	CCS811_I2C_SDA_1();	//CPU释放SDA总线
	Delay(10);
	CCS811_I2C_SCL_1();	//CPU驱动SCL = 1, 此时器件会返回ACK应答
	Delay(10);
	//转换输入
	GPIO_SET_INPUT(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT);
	if (CCS811_I2C_SDA_READ())	//CPU读取SDA口线状态
		re = 1;
	else
		re = 0;
	CCS811_I2C_SCL_0();
	GPIO_SET_OUTPUT(CCS811_I2C_SDA_PORT,CCS811_I2C_SDA_BIT);
	CCS811_I2C_SDA_1();	//CPU释放SDA总线
	Delay(10);
	return re;
}
void I2C_CCS811_Ack(void)
{
	CCS811_I2C_SDA_0();	//CPU驱动SDA = 0
	Delay(10);
	CCS811_I2C_SCL_1();	//CPU产生1个时钟
	Delay(10);
	CCS811_I2C_SCL_0();
	Delay(10);
	CCS811_I2C_SDA_1();	//CPU释放SDA总线
}
void I2C_CCS811_NAck(void)
{
	CCS811_I2C_SDA_1();	//CPU驱动SDA = 1
	Delay(10);
	CCS811_I2C_SCL_1();	//CPU产生1个时钟
	Delay(10);
	CCS811_I2C_SCL_0();
	Delay(10);	
}
//写单个数据
unsigned char CCS811_WriteI2C(unsigned char Slave_Addr,unsigned char REG_Addr,unsigned char data)
{
	I2C_CCS811_Start();  //起始信号
                I2C_CCS811_SendByte(Slave_Addr);	//发送设备地址+写信号
 	if(I2C_CCS811_WaitAck()==1)
	return 1;
	I2C_CCS811_SendByte(REG_Addr);	//发送内部寄存器地址，
 	if(I2C_CCS811_WaitAck()==1)
	return 2;
	I2C_CCS811_SendByte(data);		//发送内部寄存器数据，
	if(I2C_CCS811_WaitAck()==1)
	return 3;
	I2C_CCS811_Stop();			//发送停止信号
	return 0;
}
//写多个数据
unsigned char CCS811_MulWriteI2C(unsigned char Slave_Addr,unsigned char REG_Addr,unsigned char const *data,unsigned char length)
{
	I2C_CCS811_Start(); //起始信号
                I2C_CCS811_SendByte(Slave_Addr);	//发送设备地址+写信号
 	if(I2C_CCS811_WaitAck()==1)
	return 1;
                I2C_CCS811_SendByte(REG_Addr);	//发送内部寄存器地址，
 	if(I2C_CCS811_WaitAck()==1)
	return 2;
	while(length)
	{
		I2C_CCS811_SendByte(*data++);	//发送内部寄存器数据，
	                if(I2C_CCS811_WaitAck()==1)
		return 3;          //应答
		length--;
	}
	I2C_CCS811_Stop();		//发送停止信号		
		return 0;

//从IIC设备读取一个字节数据
unsigned char CCS811_ReadI2C(unsigned char Slave_Addr,unsigned char REG_Addr,unsigned char *REG_data,unsigned char length)
{
                I2C_CCS811_Start();		//起始信号
	I2C_CCS811_SendByte(Slave_Addr);	//发送设备地址+写信号
 	if(I2C_CCS811_WaitAck()==1)		//等待应答
		return 1;
	I2C_CCS811_SendByte(REG_Addr);	//发送存储单元地址
 	if(I2C_CCS811_WaitAck()==1)
		return 2;
	I2C_CCS811_Stop();           
                I2C_CCS811_Start();//起始信号
	I2C_CCS811_SendByte(Slave_Addr+1);	//发送设备地址+读信号
 	if(I2C_CCS811_WaitAck()==1)
		return 3;
	while(length-1)
	{
		*REG_data++=I2C_CCS811_ReadByte();	//读出寄存器数据
		I2C_CCS811_Ack();//应答
		length--;
	}
	*REG_data=I2C_CCS811_ReadByte();  
	I2C_CCS811_NAck();		//发送停止传输信号
	I2C_CCS811_Stop();			//停止信号
		return 0;
}
void CCS811Init()
{
	unsigned char idCount = 0;               // count the correct times of id.
	CCS811_Config();
                I2C_CCS811_Stop();  
                CCS811_CS_ON(); 					   	//nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low
	Delay_ms(100);
	// get CCS811 device id,when addr pin connect to GND and the id is 0x81(129)
	while( FlagGetId)
	{
		CCS811_ReadI2C(CCS811_Add, 0x20, Information, 1); //Read CCS's information  ,ID
		if(Information[0] == 0x81)
		{
			if(++idCount == n)
			{
				FlagGetId = 0;
			}
			else
			{
				printf("id=%d,correct %d!\r\n", Information[0], idCount);
			}
		}
		else
		{
			printf("id=%d,incorrect,continuing...\r\n", Information[0]);
		}
		Delay_ms(100);
	}
	printf("id correct,initing...\r\n");
	Delay_ms(100);
	CCS811_ReadI2C(CCS811_Add, 0x23, &Information[1], 2);	  //FW_Boot_Version
	Delay_ms(100);
	CCS811_ReadI2C(CCS811_Add, 0x24, &Information[3], 2); 	//FW_App_Version
	Delay_ms(100);
	CCS811_ReadI2C(CCS811_Add, 0x00, &Status, 1);	          //Firstly the status register is read and the APP_VALID flag is checked.
	Delay_ms(100);
	// if there is valid application firmware loaded
	if(Status & 0x10)
	{
		while(!(Status & 0x80)) // if firmware not in application mode but boot mode.
		{
			CCS811_WriteI2C_byte(CCS811_Add, 0xF3, 0xF0);	      // Application Verify
			printf("trying to transition the CCS811 state from boot to application mode...\r\n");
			CCS811_MWriteI2C_byte(CCS811_Add, 0xF4, &temp, 0);	//Used to transition the CCS811 state from boot to application mode, a write with no data is required.
			Delay_ms(100);
			CCS811_ReadI2C(CCS811_Add, 0x00, &Status, 1);
			Delay_ms(100);
		}
		printf("CCS811 is already in application mode!\r\n");
	}
	Delay_ms(100);
	CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1);
	Delay_ms(100);
	MeasureMode &= 0x70;                                       // get measure mode
	//if measure mode incorrect,and reset the measure mode.
	while(MeasureMode != DRIVE_MODE_1SEC)
	{
		CCS811_WriteI2C_byte(CCS811_Add, 0x01, DRIVE_MODE_1SEC); // Write Measure Mode Register,sensor measurement every second,no interrupt
		Delay_ms(100);
		CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1);
		MeasureMode &= 0x70;
		printf("trying to enter measure mode...\r\n");
		Delay_ms(100);
	}
	Delay_ms(100);
	CCS811_ReadI2C(CCS811_Add, 0x00, &Status, 1);
	Delay_ms(100);
	CCS811_ReadI2C(CCS811_Add, 0x01, &MeasureMode, 1);
	Delay_ms(100);
	CCS811_CS_OFF();
	Delay_ms(100);
	CCS811_ReadI2C(CCS811_Add, 0xE0, &Error_ID, 1);
	printf("status=%d error_id=%d measureMode=%d \r\n", Status, Error_ID, MeasureMode);
}
void CCS811GetData()
{
	CCS811_CS_ON(); 	// nWAKE pin is asserted at least 50μs before the transaction and kept asserted throughout,nWAKE pin is active low
	Delay_ms(10);
	CCS811_ReadI2C(CCS811_Add, 0x02, BUF, 8);
	Delay_ms(10);
	CCS811_ReadI2C(CCS811_Add, 0x20, Information, 1); // Read CCS's information  ,ID
	Delay_ms(10);
	CCS811_ReadI2C(CCS811_Add, 0xE0, &Error_ID, 1);
	CCS811_CS_OFF();
	eco2 = (unsigned int)BUF[0] * 256 + BUF[1];
	tvoc = (unsigned int)BUF[2] * 256 + BUF[3];
	device_id  = Information[0];
	error_id  = Error_ID;
	Error_ID = 0;
	Information[0] = 0;
}
void CCS811ClearData()
{
	device_id = 0;
	eco2 = 0;
	status = 0;
	tvoc = 0;
	error_id = 0;
}