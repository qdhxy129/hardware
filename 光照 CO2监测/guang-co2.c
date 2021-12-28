
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
#include "tcp-socket.h"                     //tcp通信协议api
#include "udp-socket.h"                     //udp通信api
#include "http-socket/http-socket.h"        //http socket通信api
#include <stdlib.h>                         //标准的系统相关接口
#include <stdio.h>                          //标准设备的输入输出相关接口
#include <string.h>                         //字符串处理库
#include "dev/i2c-bh1750-arch.h"     	    //光照传感器接口
#include "dev/i2c-CCS811-arch.h"     	    //光照传感器接口
#include "dev/lcd-24.h"			    //2.4寸LCD模块驱动接口
#include "dev/pic-scan.h"
//定义通信端口
#define PORT 8002
//static float t=0;
//tcp socket 数据对象,用来保存tcp通信相关的数据结构
static struct tcp_socket s;
//输入缓冲区为400个字节
#define INPUTBUFSIZE 400
static uint8_t inputbuf[INPUTBUFSIZE];
//输出缓冲区为400个字节
#define OUTPUTBUFSIZE 400
//定义输出缓冲区
static uint8_t outputbuf[OUTPUTBUFSIZE];
//ds6配置
static struct uip_ds6_notification n;
static uint8_t notiok=0; 
static uint8_t sendok=0; 
//定义定时器对象,保存定时器的相关参数
static struct etimer periodic_timer_tcp;
static struct etimer periodic_timer_sensor1;
static struct etimer periodic_timer_sensor2;
static struct etimer periodic_timer_lcd;
//自定义输出数组
static unsigned char LUX1CO2[20];
static unsigned char lux2[6];
static unsigned char eco22[6];
static unsigned char tvoc2[6];
//声明进程
PROCESS(tcp_link, "tcp_link");
PROCESS(bh1750, "bh1750 process");
PROCESS(ccs811, "ccs811 process");
PROCESS(lcd24_process, "lcd-2.4");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&bh1750,&ccs811,&tcp_link,&lcd24_process);
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
}
//输入处理函数
static int
input(struct tcp_socket *s, void *ptr,
      const uint8_t *inputptr, int inputdatalen)
{
  	sendok = 0x11;
  	printf("input %d bytes '%s' - closing socket\n", inputdatalen, inputptr);
  	tcp_socket_close(s);
  	return 0;
}
//事件函数
static void
event(struct tcp_socket *s, void *ptr,
      tcp_socket_event_t ev)
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
static void lux_0(void)
{
	if(*(lux2) == '0') {
	*(lux2) =' ';  //首字符为0，改为空格
		if(*(lux2+1) == '0') { //第2个字符为0，改为空格
		*(lux2+1) =' ';
			if(*(lux2+2) == '0') { //第3个字符为0，改为空格
			*(lux2+2) =' ';
				if(*(lux2+3) == '0') { //第3个字符为0，改为空格
				*(lux2+3) =' ';
				}
			}
		}
	}
}
static void eco2_0(void)
{
	if(*(eco22) == '0') {
	*(eco22) =' ';  //首字符为0，改为空格
		if(*(eco22+1) == '0') { //第2个字符为0，改为空格
		*(eco22+1) =' ';
			if(*(eco22+2) == '0') { //第3个字符为0，改为空格
			*(eco22+2) =' ';
				if(*(eco22+3) == '0') { //第3个字符为0，改为空格
				*(eco22+3) =' ';
				}
			}
		}
	}
}
static void tvoc_0(void)
{
	if(*(tvoc2) == '0') {
	*(tvoc2) =' ';  //首字符为0，改为空格
		if(*(tvoc2+1) == '0') { //第2个字符为0，改为空格
		*(tvoc2+1) =' ';
			if(*(tvoc2+2) == '0') { //第3个字符为0，改为空格
			*(tvoc2+2) =' ';
				if(*(tvoc2+3) == '0') { //第3个字符为0，改为空格
				*(tvoc2+3) =' ';
				}
			}
		}
	}
}
void static light(uint16_t color)
{
  	LCD_DrawLine(41,138,47,138,color);
  	LCD_DrawLine(38,139,49,139,color);
  	LCD_DrawLine(38,140,50,140,color);
  	LCD_DrawLine(37,141,51,141,color);
  	LCD_DrawLine(36,142,52,142,color);
  	LCD_DrawLine(35,143,52,143,color);
  	LCD_DrawLine(34,144,53,144,color);
  	LCD_DrawLine(34,145,53,145,color);
  	LCD_DrawLine(34,146,54,146,color);
  	LCD_DrawLine(34,147,54,147,color);
  	LCD_DrawLine(34,148,54,148,color);
  	LCD_DrawLine(34,149,54,149,color);
  	LCD_DrawLine(34,150,54,150,color);
  	LCD_DrawLine(35,151,53,151,color);
  	LCD_DrawLine(35,152,53,152,color);
  	LCD_DrawLine(36,153,52,153,color);
  	LCD_DrawLine(36,154,51,154,color);
  	LCD_DrawLine(37,155,51,155,color);
  	LCD_DrawLine(38,156,50,156,color);
  	LCD_DrawLine(38,157,50,157,color);
  	LCD_DrawLine(39,158,49,158,color);
  	LCD_DrawLine(40,159,48,159,color);
  	LCD_DrawLine(40,160,48,160,color);
  	LCD_DrawLine(40,161,48,161,color);
  	LCD_DrawLine(40,162,48,162,color);
  	LCD_DrawLine(40,163,48,163,color);
}
//进程实现
PROCESS_THREAD(tcp_link, ev, data)
{
  	//ipv6地址设置
  	uip_ip6addr_t ip6addr;
  	//ipv4地址设置
  	uip_ip4addr_t ip4addr;
  	//进程开始
  	PROCESS_BEGIN();
  	//打开所有led
  	leds_on(LEDS_ALL);
  	//添加回调函数
  	uip_ds6_notification_add(&n, route_callback);
  	//定时器时间设置
  	etimer_set(&periodic_timer_tcp, 10 * CLOCK_SECOND);
  	while(1){
   		//等待事件定时器的事件产生
   		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer_tcp));
   		//复位事件定时器
    		etimer_reset(&periodic_timer_tcp);
    		*(LUX1CO2) = '0';
    		*(LUX1CO2+1) = '3';
    		*(LUX1CO2+2) = (char)('0'+eco2/10000);
    		*(LUX1CO2+3) = (char)('0'+eco2%10000/1000);
    		*(LUX1CO2+4) = (char)('0'+eco2%1000/100);
    		*(LUX1CO2+5) = (char)('0'+eco2%100/10);
    		*(LUX1CO2+6) = (char)('0'+eco2%10);
    		*(LUX1CO2+7) = '0';
    		*(LUX1CO2+8) = '4';
    		*(LUX1CO2+9) = lux/10000+0x30;
    		*(LUX1CO2+10) = (lux%10000)/1000+0x30;
    		*(LUX1CO2+11) = (lux%1000)/100+0x30;
    		*(LUX1CO2+12) = (lux%100)/10+0x30;
    		*(LUX1CO2+13) = lux%10+0x30;
    		*(LUX1CO2+14) = (char)('0'+tvoc/10000);
    		*(LUX1CO2+15) = (char)('0'+tvoc%10000/1000);
    		*(LUX1CO2+16) = (char)('0'+tvoc%1000/100);
    		*(LUX1CO2+17) = (char)('0'+tvoc%100/10);
    		*(LUX1CO2+18) = (char)('0'+tvoc%10);
    		*(LUX1CO2+19) = '\0';
    		printf("%s",LUX1CO2);
    		//添加ipv4服务器地址
			//添加ipv6服务器地址
			uip_ip6addr(&ip6addr, 0x2001,0xda8,0x270,0x2020,0xf816,0x3eff,0xfe88,0xdb99);
    		//tcp socket 通信注册
    		tcp_socket_register(&s, NULL,
                	inputbuf, sizeof(inputbuf),
                	outputbuf, sizeof(outputbuf),
                	input, event);
    		//tcp socket连接服务器
    		tcp_socket_connect(&s, &ip6addr, PORT);
    		//tcp发送光照CO2数据到服务器
    		tcp_socket_send_str(&s,(char*)LUX1CO2);    
    		//断开
    		tcp_socket_close(&s);
  	}
  	//进程结束
  	PROCESS_END();
}
PROCESS_THREAD(bh1750,ev,data)
{
  	//进程开始
  	PROCESS_BEGIN();
  	//定时器时间设置
  	etimer_set(&periodic_timer_sensor1, 2 * CLOCK_SECOND); 
  	//初始化传感器
  	BH1750_Init();
  	while (1)
  	{
   		//等待事件定时器的事件产生
    		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer_sensor1));
    		//复位事件定时器
    		etimer_reset(&periodic_timer_sensor1);
    		//数据采集
    		BH1750GetData();
  	}
  	//进程结束
  	PROCESS_END();
}
PROCESS_THREAD(ccs811, ev, data)
{
  	//进程开始
  	PROCESS_BEGIN();
  	//定时器时间设置
  	etimer_set(&periodic_timer_sensor2, 2 * CLOCK_SECOND); 
  	//初始化传感器
  	CCS811Init();
  	while (1)
  	{
    		//等待事件定时器的事件产生
    		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer_sensor2));
    		//复位事件定时器
    		etimer_reset(&periodic_timer_sensor2);
    		//数据采集
    		CCS811GetData();
  	}
  	//进程结束
  	PROCESS_END();
}
//进程实现
PROCESS_THREAD(lcd24_process, ev, data)
{ 
  	//进程开始
  	PROCESS_BEGIN();
  	//定时器时间设置
  	etimer_set(&periodic_timer_lcd, 2 * CLOCK_SECOND);
  	while(1)
  	{
    		//等待事件定时器的事件产生
    		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer_lcd));
    		//复位事件定时器
    		etimer_reset(&periodic_timer_lcd);
    		if(notiok != 0){
      		LCD_ShowPicture(10,2,34,26,gImage_wifi);
      		if(sendok != 0){
        		LCD_ShowPicture(147,2,230,26,gImage_plane);
      			       }
    		}else{
      LCD_Fill(10,2,34,26,BLACK);
    }   
    //生成光照值
    *(lux2) = lux/10000+0x30;
    *(lux2+1) = (lux%10000)/1000+0x30;
    *(lux2+2) = (lux%1000)/100+0x30;
    *(lux2+3) = (lux%100)/10+0x30;
    *(lux2+4) = lux%10+0x30;
    *(lux2+5) = '\0';
    lux_0();
    //生成CO2值
    *(eco22) = (char)('0'+eco2/10000);
    *(eco22+1) = (char)('0'+eco2%10000/1000);
    *(eco22+2) = (char)('0'+eco2%1000/100);
    *(eco22+3) = (char)('0'+eco2%100/10);
    *(eco22+4) = (char)('0'+eco2%10);
    *(eco22+5) = '\0';
    eco2_0();
    //生成有机物值
    *(tvoc2) = (char)('0'+tvoc/10000);
    *(tvoc2+1) = (char)('0'+tvoc%10000/1000);
    *(tvoc2+2) = (char)('0'+tvoc%1000/100);
    *(tvoc2+3) = (char)('0'+tvoc%100/10);
    *(tvoc2+4) = (char)('0'+tvoc%10);
    *(tvoc2+5) = '\0';
    tvoc_0();
    //光照值
    lcdbuff=(unsigned char *)lux2;
    LCD_ShowString(90,140,lcdbuff,YELLOW,BLACK,32);
    if(lux<1000){light(YELLOW);}
    if(lux<4000&&lux>=1000){light(GREEN);}
    if(lux<10000&&lux>=4000){light(BROWN);}
    if(lux>=10000){light(RED);}
    //CO2值
    lcdbuff=(unsigned char *)eco22;
    LCD_ShowString(90,190,lcdbuff,YELLOW,BLACK,32);
    //有机物值
    lcdbuff=(unsigned char *)tvoc2;
    LCD_ShowString(90,250,lcdbuff,YELLOW,BLACK,32);
  }
  //进程结束
  PROCESS_END();
}