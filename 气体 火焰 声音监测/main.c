
#include "contiki.h"                        //contiki系统api库
#include "contiki-net.h"                    //contiki网络api库
#include "contiki-lib.h"                    //contiki系统lib库
#include "lib/random.h"                     //随机数生成库
#include "sys/ctimer.h"                     //系统回调定时器库
#include "sys/etimer.h"                     //系统事件定时器库
#include "net/rpl/rpl.h"                    //路由相关api
#include "net/ip/uip.h"                     //uip网络协议api
#include "net/ipv6/uip-ds6.h"               //ipv6相关api
#include "dev/leds.h"                       //LED驱动库
#include "aes.h"                            //aes库
#include "ip64-addr.h"                      //ip64地址接口库 
#include "apps/mdns/mdns.h"                 //mdns接口
#include "apps/simple-rpl/simple-rpl.h"     //rpl相关
#include "udp-socket.h"                     //udp通信api
#include "http-socket/http-socket.h"        //http socket通信api
#include <stdlib.h>                         //标准的系统相关接口
#include <stdio.h>                          //标准设备的输入输出相关接口
#include <string.h>                         //字符串处理库
#include "dev/lcd-24.h"			//2.4寸LCD模块驱动接口
#include "dev/pic.h"            //图片加载
//#include "dev/pic1.h"            //图片加载
#include "dev/soc-adc.h"
#include "gpio-arch.h"         //GPIO
#include "afont.h"
#include "dev/adc.h"  //adc接口
//定义通信端口
#define PORT 8002
//tcp socket 数据结构体
static struct tcp_socket s;
//输入缓冲区为400个字节
#define INPUTBUFSIZE 400
static uint8_t inputbuf[INPUTBUFSIZE];
//输出缓冲区为400个字节
#define OUTPUTBUFSIZE 400
//定义输出缓冲区
static uint8_t outputbuf[OUTPUTBUFSIZE];
//tcp socket 数据对象,用来保存tcp通信相关的数据结构
static struct tcp_socket s;
static struct uip_ds6_notification n;
//定义温度和湿度变量
static unsigned char buf0[7]={0}, buf[22]={0};
static unsigned char buf1[7]={0},bufMQ135H[7]={0},bufMQ135L[7]={0}; 
static uint8_t notiok=0;
static uint16_t MQ135,MQ7;
static uint32_t mq7value,mq135value;
//ipv6地址设置
uip_ip6addr_t ip6addr;
//ipv4地址设置
uip_ip4addr_t ip4addr;
//路由回调函数,处理路由事件
static void
route_callback(int event, uip_ipaddr_t *route, uip_ipaddr_t *ipaddr,
	int numroutes)
{
	if(event == UIP_DS6_NOTIFICATION_DEFRT_ADD) {
		leds_off(LEDS_ALL);
		printf("Got a RPL route\n");
		notiok = 0x11;
	}
	else
		notiok = 0;
}

/*---------------------------------------------------------------------------*/
//输入处理函数
static int
input(struct tcp_socket *s, void *ptr,
	const uint8_t *inputptr, int inputdatalen)
{
	printf("input %d bytes '%s' - closing socket\n", inputdatalen, inputptr);
	tcp_socket_close(s);
	return 0;
}
//事件函数
static void
event(struct tcp_socket *s, void *ptr,tcp_socket_event_t ev)
{
	if(ev == TCP_SOCKET_CONNECTED) {
		printf("Socket connected\n");
	} else if(ev == TCP_SOCKET_DATA_SENT) {
		printf("Socket data was sent\n");
	} else if(ev == TCP_SOCKET_CLOSED) {
		printf("Socket closed\n");
	} else if(ev == TCP_SOCKET_ABORTED) {
		printf("Socket reset\n");
	} else if(ev == TCP_SOCKET_TIMEDOUT) {
		printf("Socket timedout\n");
	}
}
 //校验函数
unsigned int 
CHK(unsigned char *check, unsigned int length)
{
	unsigned int i,sum=0;
	for(i=0;i<length;i++)
	{
		sum+=check[i];
	}
	return sum;
}
//进程声明
PROCESS(lcd24_process, "lcd-2.4");
PROCESS(sensor, "sensor");
PROCESS(lcd_HY, "HY");
PROCESS(lcd_CO, "CO");
PROCESS(lcd_MQ135, "MQ135");
PROCESS(tcp_link, "tcp_link");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&lcd24_process,&tcp_link,&sensor,&lcd_HY,&lcd_CO,&lcd_MQ135);
//lcd24进程实现
PROCESS_THREAD(lcd24_process, ev, data)
{       
	//进程开始
	PROCESS_BEGIN();
	// 定时器
	static struct etimer lcd_timer;
	//定时器时间设置
	etimer_set(&lcd_timer, CLOCK_SECOND);
	void delay(int time)
	{
		while(time--);
	}
	
	while(1)
	{
		
	//等待定时器时间到
	PROCESS_WAIT_UNTIL(etimer_expired(&lcd_timer));
        	//重置定时器
	etimer_reset(&lcd_timer);
	if(YW_Status()==1)
	{     
		LCD_ShowPicture(10,40,140,80,gImage_fume);
	}
	else
	{            
		LCD_ShowPicture(10,40,140,80,gImage_nofume);
	}

	if(notiok != 0)
		LCD_ShowPicture(280,0,40,40,gImage_YesInternet);	    
	else
		LCD_ShowPicture(280,0,40,40,gImage_NoInternet);
	}
	//进程结束
	PROCESS_END();
}
//sensor进程实现
PROCESS_THREAD(sensor, ev, data)
{ 
	//进程开始
	PROCESS_BEGIN();
	// 定时器
	static struct etimer sensor_timer;
	//定时器时间设置
	etimer_set(&sensor_timer, CLOCK_SECOND);
	while(1)
	{	    	
		//等待定时器时间到
		PROCESS_WAIT_UNTIL(etimer_expired(&sensor_timer));
       		 //重置定时器
		etimer_reset(&sensor_timer);	
		//获得adc的数值 MQ7   PA7
		MQ7=adc_get(SOC_ADC_ADCCON_CH_AIN0+ADC_MQ7_OUT_PIN, SOC_ADC_ADCCON_REF_AVDD5, SOC_ADC_ADCCON_DIV_512)>>4;   
		mq7value=MQ7*3300/4096;
		printf("MQ7     %d.%dppm\r\n",mq7value/100,mq7value%100);
  		//获得adc的数值 MQ135   PA6
		MQ135=adc_get(SOC_ADC_ADCCON_CH_AIN0+ADC_MQ135_OUT_PIN, SOC_ADC_ADCCON_REF_AVDD5, SOC_ADC_ADCCON_DIV_512)>>4;   
		if(MQ135>1288)
		{MQ135=1288;}
		mq135value=MQ[MQ135];
		printf("MQ135   %d.%dppm\r\n",MQ[MQ135]/1000,MQ[MQ135]%1000);
	}
 	//进程结束
	PROCESS_END();
}
//HY进程实现
PROCESS_THREAD(lcd_HY, ev, data)
{ 
	//进程开始
	PROCESS_BEGIN();
	// 定时器
	static struct etimer tcp_timer;
	//定时器时间设置
	etimer_set(&tcp_timer, CLOCK_SECOND);
	while(1)
	{	    
		//等待定时器时间到
		PROCESS_WAIT_UNTIL(etimer_expired(&tcp_timer));
        		//重置定时器
		etimer_reset(&tcp_timer);
		if(HY_Status()==1)
		{      
			LCD_ShowPicture(170,40,140,80,gImage_fire);
		}
		else
		{             
			LCD_ShowPicture(170,40,140,80,gImage_nofire);
		}
		
	}
  	//进程结束
	PROCESS_END();
}
//HY进程实现
PROCESS_THREAD(lcd_CO, ev, data)
{ 
	//进程开始
	PROCESS_BEGIN();
	// 定时器
	static struct etimer tcp_timer;
	//定时器时间设置
	etimer_set(&tcp_timer, CLOCK_SECOND);
	while(1)
	{	   	
		//等待定时器时间到
		PROCESS_WAIT_UNTIL(etimer_expired(&tcp_timer));
        		//重置定时器
		etimer_reset(&tcp_timer);		
 		//MQ7
		if((mq7value/100)>70)
		{
			LCD_ShowPicture(5,200,40,40,gImage_red);
		} 
		else          LCD_ShowPicture(5,200,40,40,gImage_green);
		*buf0=(mq7value/100)/10;
		*(buf0+1)=(mq7value/100)%10;
		LCD_ShowChinese(20,155,2,buf0,BLACK,WHITE,16);
		LCD_ShowString(73,155,(unsigned char *)".",BLACK,WHITE,32);
		*buf1=(mq7value%100)/10;
		*(buf1+1)=(mq7value%100)%10;
		LCD_ShowChinese(90,155,2,buf1,BLACK,WHITE,16);
		LCD_ShowString(80,200,(unsigned char *)"ppm",BLACK,WHITE,32);
	}
  	//进程结束
	PROCESS_END();
}
//HY进程实现
PROCESS_THREAD(lcd_MQ135, ev, data)
{ 
	//进程开始
	PROCESS_BEGIN();
	// 定时器
	static struct etimer tcp_timer;
	//定时器时间设置
	etimer_set(&tcp_timer, CLOCK_SECOND);
	while(1)
	{	    	
		//等待定时器时间到
		PROCESS_WAIT_UNTIL(etimer_expired(&tcp_timer));
        		//重置定时器
		etimer_reset(&tcp_timer);
  		//获得adc的数值 MQ135   PA6
		if((MQ[MQ135]/1000)>400)  LCD_ShowPicture(165,200,40,40,gImage_red);
		else                LCD_ShowPicture(165,200,40,40,gImage_green);
		*bufMQ135H=(mq135value/1000)/100;
		*(bufMQ135H+1)=(mq135value/1000)/10%10;   
		*(bufMQ135H+2)=(mq135value/1000)%10;
		LCD_ShowChinese(180,155,3,bufMQ135H,BLACK,WHITE,16);
		LCD_ShowString(230,155,(unsigned char *)".",BLACK,WHITE,32);
		*bufMQ135L=(mq135value%1000)/100;
		*(bufMQ135L+1)=(mq135value%1000)/10%10;   
		*(bufMQ135L+2)=(mq135value%1000)%10;
		LCD_ShowChinese(250,155,3,bufMQ135L,BLACK,WHITE,16);
		LCD_ShowString(240,200,(unsigned char *)"ppm",BLACK,WHITE,32);			 
	}
  	//进程结束
	PROCESS_END();
}
//tcp进程实现
PROCESS_THREAD(tcp_link, ev, data)
{ 
	//进程开始
	PROCESS_BEGIN();
  	//添加ipv6服务器地址
  	uip_ip6addr(&ip6addr, 0x2001,0xda8,0x270,0x2020,0xf816,0x3eff,0xfe88,0xdb99);
	//打开所有led
	leds_on(LEDS_ALL);
	//添加回调函数
	uip_ds6_notification_add(&n, route_callback);
	// 定时器
	static struct etimer tcp_timer;
	//定时器时间设置
	etimer_set(&tcp_timer, CLOCK_SECOND*10);
	while(1)
	{	    	
		//等待定时器时间到
		PROCESS_WAIT_UNTIL(etimer_expired(&tcp_timer));
        		//重置定时器
		etimer_reset(&tcp_timer);
		unsigned char tcpmq7=0;
		//tcp socket 通信注册
		tcp_socket_register(&s, NULL,
		inputbuf, sizeof(inputbuf),
		outputbuf, sizeof(outputbuf),
		input, event);
		if(YW_Status()==1)
		{
			*buf='0';
			*(buf+1)='6';
			*(buf+2)='1';
		}
		else
		{
			*buf='0';
			*(buf+1)='6';
			*(buf+2)='0';
		}

		if(HY_Status()==1)
		{
			*(buf+3)='0';
			*(buf+4)='7';
			*(buf+5)='1';
		}
		else
		{
			*(buf+3)='0';
			*(buf+4)='7';
			*(buf+5)='0';
		}
		*(buf+6)='0';
		*(buf+7)='8'; 
		*(buf+8)=(mq7value/100)/10+0x30;
		*(buf+9)=(mq7value/100)%10+0x30;
		*(buf+10)='.';
		*(buf+11)=(mq7value%100)/10+0x30;       
		*(buf+12)=(mq7value%100)%10+0x30;
		*(buf+13)='0';
		*(buf+14)='9';
		*(buf+15)=(mq135value/1000)/100+0x30;
		*(buf+16)=(mq135value/1000)/10%10+0x30;
		*(buf+17)=(mq135value/1000)%10+0x30;
		*(buf+18)='.';
		*(buf+19)=(mq135value%1000)/100+0x30;
		*(buf+20)=(mq135value%1000)/10%10+0x30;
		*(buf+21)=(mq135value%1000)%10+0x30;
		//tcp socket连接服务器
		tcp_socket_connect(&s, &ip6addr, PORT);
		//tcp发送数据buf到服务器
		tcp_socket_send_str(&s,buf);
        		//发送完打印到串口
		printf("%s\r\n",buf);
		tcp_socket_close(&s);
	}
  	//进程结束
	PROCESS_END();
}