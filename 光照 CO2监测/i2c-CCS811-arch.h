
#ifndef I2C_CCS811_ARCH_H
#define I2C_CCS811_ARCH_H
//SCL
#define CCS811_I2C_SCL_PORT GPIO_D_BASE
#define CCS811_I2C_SCL_BIT   (1 << 1)	
//SDA
#define CCS811_I2C_SDA_PORT GPIO_D_BASE
#define CCS811_I2C_SDA_BIT   (1 << 3)
//aWAKE
#define CCS811_aWAKE_PORT GPIO_D_BASE
#define CCS811_aWAKE_BIT   (1 << 4)
#define CCS811_I2C_Write	0xB4		//写控制bit
#define CCS811_I2C_Read	0xB5		//读控制bit
#define CCS811_Add      		0x5A<<1
#define STATUS_REG      		0x00
#define MEAS_MODE_REG   		0x01
#define ALG_RESULT_DATA 		0x02
#define ENV_DATA        		0x05
#define NTC_REG         		0x06
#define THRESHOLDS      		0x10
#define BASELINE 			0x11
#define HW_ID_REG 		0x20
#define ERROR_ID_REG 		0xE0
#define APP_START_REG 		0xF4
#define SW_RESET 			0xFF
#define CCS811_ADDRESS 		0x5A
#define GPIO_WAKE		0x5
#define DRIVE_MODE_IDLE 		0x0   // 不测量
#define DRIVE_MODE_1SEC 		0x10  // 1秒间隔测量
#define DRIVE_MODE_10SEC		0x20  // 10s间隔测量
#define DRIVE_MODE_60SEC 		0x30  // 每分钟测量
#define INTERRUPT_DRIVEN 		0x8   // 强力测量250ms
#define THRESHOLDS_ENABLED 	0x4
unsigned int eco2;
unsigned int tvoc;
unsigned char status;
unsigned char device_id;
unsigned char error_id;
unsigned int raw_data;
unsigned char CCS811_ReadI2C(unsigned char Slave_Addr,unsigned char REG_Addr,unsigned char *REG_data,unsigned char length);
unsigned char CCS811_MulWriteI2C(unsigned char Slave_Addr,unsigned char REG_Addr,unsigned char const *data,unsigned char length);
unsigned char CCS811_WriteI2C(unsigned char Slave_Addr,unsigned char REG_Addr,unsigned char data);
unsigned char I2C_WaitAck(void);
unsigned char I2C_ReadByte(void);
void CCS811_Config(void);
void CCS811_I2C_Stop(void);
void CCS811Init(void);           
void CCS811GetData(void);
void CCS811ClearData(void);
void I2C_CC811_NAck(void);
void I2C_CC811_Ack(void);
void I2C_CC811_Send(unsigned char cucByte);
void I2C_CC811_Start(void);
void I2C_CC811_Stop(void);
#endif