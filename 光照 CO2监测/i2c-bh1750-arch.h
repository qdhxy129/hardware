#ifndef __I2C_BH1750_ARCH_H__
#define __I2C_BH1750_ARCH_H__
//SCL
#define BH1750_I2C_SCL_PORT GPIO_C_BASE
#define BH1750_I2C_SCL_BIT   (1 << 4)	
//SDA
#define BH1750_I2C_SDA_PORT GPIO_C_BASE
#define BH1750_I2C_SDA_BIT   (1 << 5)
//BH1750的地址
#define BH1750_Addr		0x46
//BH1750指令码
#define POWER_OFF		0x00
#define POWER_ON		                0x01
#define MODULE_RESET		0x07
#define CONTINUE_H_MODE	                0x10
#define CONTINUE_H_MODE2	0x11
#define CONTINUE_L_MODE		0x13
#define ONE_TIME_H_MODE		0x20
#define ONE_TIME_H_MODE2	                0x21
#define ONE_TIME_L_MODE		0x23
//测量模式
#define Measure_Mode		CONTINUE_H_MODE
#define BH1750_I2C_Write	0		//写控制bit
#define BH1750_I2C_Read	1		//读控制bit
unsigned int lx;
void Delay(unsigned int time);
void Delay_ms(unsigned int Time);
void BH1750_Init(void);			//未包含IIC初始化
unsigned char BH1750_Byte_Write(unsigned char data);
unsigned int BH1750_Read_Measure(void);
void LightGetData(void);
void BH1750_PowerON(void);
void BH1750_PowerOFF(void);
void BH1750_RESET(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(unsigned char _ucByte);
unsigned char I2C_ReadByte(void);
unsigned char I2C_WaitAck(void);
void I2C_Ack(void);
void I2C_NAck(void);
unsigned char I2C_CheckDevice(unsigned char _Address);
#endif