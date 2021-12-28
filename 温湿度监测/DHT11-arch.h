#ifndef __DHT11_H
#define __DHT11_H
extern void Delay_ms(unsigned int xms);	    //延时
extern void Start_DHT11(void);              //启动温湿度传感器    
extern void DHT11_Inint(void);              //初始化温湿度传感器 
extern unsigned char temp[2];               //保存温度值
extern unsigned char humidity[2];           //保存湿度值
extern unsigned char Hum_dec,Hum_uni,Tem_dec,Tem_uni,Tem_data_H,RH_data_H;
#endif
