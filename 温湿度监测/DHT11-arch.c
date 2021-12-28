#include "dev/gpio.h"   //gpio定义
#include "clock.h"      //时钟库
//接口相关定义
#define DHT11_DATA_PORT   GPIO_B_BASE
#define DHT11_DATA_BIT    (1 << 5)
#define DHT11_DATA        DHT11_DATA_PORT, DHT11_DATA_BIT  
void Delay_us(void);                //us延时
void Delay_ms(unsigned int Time);   //ms延时
void Start_DHT11(void);             //开始读取温度
void DHT11_Inint(void);             //温湿度传感器初始化
//温湿度传感器相关的变量定义
unsigned char DHT11_FLAG,DHT11_temp;
unsigned char Hum_dec,Hum_uni,Tem_dec,Tem_uni=4;
unsigned char Tem_High,Tem_Low,RH_High,RH_Low,DHT11_check;
unsigned char Tem_H_temp,Tem_L_temp,RH_H_temp,RH_L_temp,DHT11_check_temp;
unsigned char DHT11_comdata;
//gpio设置
static void
gpio_set1(int port, int bit)
{
  	REG((port | GPIO_DATA) + (bit << 2)) = bit;
}
//gpio复位
static void
gpio_reset1(int port, int bit)
{
  	REG((port | GPIO_DATA) + (bit << 2)) = 0;
}
//获得gpio数值
static int
gpio_get1(int port, int bit)
{
  	return REG((port | GPIO_DATA) + (bit << 2));
}
//us延时
static void
Delay_us(unsigned char DELAY)
{
    	clock_delay_usec(DELAY);
}
//ms延时
void Delay_ms(unsigned int Time)
{
  	unsigned char i;
  	while(Time--)
  	{
    		for(i=0;i<100;i++)
    		Delay_us(10);
  	}
}
//读取温度
void Start_DHT11(void)    
{     
    	unsigned char i;         
    	for(i=0;i<8;i++)    
    	{
        		DHT11_FLAG=2; 
        		while((!gpio_get1(DHT11_DATA))&&DHT11_FLAG++);
        		Delay_us(10);
		Delay_us(10);
		Delay_us(10);
        		DHT11_temp=0;
        		if(gpio_get1(DHT11_DATA))DHT11_temp=1;
       		DHT11_FLAG=2;
        		while((gpio_get1(DHT11_DATA))&&DHT11_FLAG++);   
        		if(DHT11_FLAG==1)break;    
        		DHT11_comdata<<=1;
        		DHT11_comdata|=DHT11_temp; 
    	}    
}
//温湿度传感器初始化
void DHT11_Inint(void)   
{
    	gpio_reset1(DHT11_DATA);
    	Delay_ms(10);  //>18MS
    	Delay_ms(10); 
    	Delay_ms(10); 
    	gpio_set1(DHT11_DATA);
    	GPIO_SET_INPUT(DHT11_DATA_PORT,DHT11_DATA_BIT);//INPUT
    	Delay_us(10);
    	Delay_us(10);
    	Delay_us(10);
    	Delay_us(5);
    	if(!gpio_get1(DHT11_DATA))
    	{
        		DHT11_FLAG=2; 
        		while((!gpio_get1(DHT11_DATA))&&DHT11_FLAG++);
        		DHT11_FLAG=2;
        		while((gpio_get1(DHT11_DATA))&&DHT11_FLAG++); 
        		Start_DHT11();
        		Delay_us_dht11(10);
        		RH_data_H_temp=DHT11_comdata;
        		Start_DHT11();
        		Delay_us_dht11(10);
        		RH_data_L_temp=DHT11_comdata;
        		Start_DHT11();
		Delay_us_dht11(10);
        		Tem_data_H_temp=DHT11_comdata;
        		Start_DHT11();
		Delay_us_dht11(10);
        		Tem_data_L_temp=DHT11_comdata;
        		Start_DHT11();
		Delay_us_dht11(10);
        		DHT11_checkdata_temp=DHT11_comdata;
        		gpio_set1(DHT11_DATA);
		Delay_us_dht11(30);
        		DHT11_temp=(Tem_data_H_temp+Tem_data_L_temp+RH_data_H_temp+RH_data_L_temp);
        		if(DHT11_temp==DHT11_checkdata_temp)
        		{
            			RH_High=RH_H_temp;
            			RH_Low=RH_L_temp;
            			Tem_High=Tem_H_temp;
            			Tem_Low=Tem_L_temp;
            			DHT11_check=DHT11_check_temp;
        		}
        		Tem_dec=Tem_High/10; 
        		Tem_uni=Tem_High%10; 
        		Hum_dec=RH_High/10; 
        		Hum_uni=RH_High%10;        
    	} 
    	else
    	{
        		Tem_dec=0; 
        		Tem_uni=0; 
        		Hum_dec=0; 
        		Hum_uni=0;  
    	} 
    	GPIO_SET_OUTPUT(DHT11_DATA_PORT,DHT11_DATA_BIT); //OUTPUT
    	Delay_us_dht11(25);
}