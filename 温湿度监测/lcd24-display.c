/*------------------------------------------------------------*/
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
#include "dev/DHT11-arch.h"                 //温湿度传感器dht11
#include <stdlib.h>                         //标准的系统相关接口
#include <stdio.h>                          //标准设备的输入输出相关接口
#include <string.h>                         //字符串处理库
#include "dev/lcd-24.h"			//2.4寸LCD模块驱动接口
/*------------------------------------------------------------*/
/******************全局定义区**************************************/
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

/***********全局静态变量*****************************************/
    //定义温度和湿度变量
	static unsigned char temhum[2]={0};
	static unsigned char tem[3]={0};
	static unsigned char hum[3]={0};
        static unsigned char buf[9]={0};
	//ipv6地址设置
	uip_ip6addr_t ip6addr;
	//ipv4地址设置
	uip_ip4addr_t ip4addr;
/*************************************************************/



//路由回调函数,处理路由事件
static void
route_callback(int event, uip_ipaddr_t *route, uip_ipaddr_t *ipaddr,
               int numroutes)
{
  if(event == UIP_DS6_NOTIFICATION_DEFRT_ADD) {
    leds_off(LEDS_ALL);
    printf("Got a RPL route\n");
  }
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


/**************************************************************/


/******************************************************************/
//进程声明
PROCESS(lcd24_process, "lcd-2.4");
PROCESS(tcp_link, "tcp_link");
PROCESS(sensor, "sensor");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&lcd24_process,&tcp_link,&sensor);
/****************************************************************/

//lcd24进程实现
PROCESS_THREAD(lcd24_process, ev, data)
{       
	//进程开始
	PROCESS_BEGIN();
	// 定时器
	static struct etimer lcd_timer;
	//定时器时间设置
	etimer_set(&lcd_timer, CLOCK_SECOND);

  while(1)
  {	    
	
	//等待定时器时间到
	PROCESS_WAIT_UNTIL(etimer_expired(&lcd_timer));
        //重置定时器
        etimer_reset(&lcd_timer);

	*(tem)=(*(temhum))/10;
	*(tem+1)=(*(temhum))%10;
	*(tem+2)='\0';

	*(hum)=(*(temhum+1))/10;
	*(hum+1)=(*(temhum+1))%10;
  	*(hum+2)='\0';

	LCD_ShowChinese(10,190,2,tem,BLUE,LIGHTBLUE,43);
	LCD_ShowChinese(170,190,2,hum,BLUE,LIGHTGREEN,43);
  }
  //进程结束
  PROCESS_END();
}
/***************************************************************/
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
       //温湿度模块初始化
        DHT11_Inint();
      if(Tem_dec!=0&&Tem_uni!=0)
      {
      if(Hum_dec!=0&&Hum_uni!=0)
      {
        *(temhum)=Tem_dec*10+Tem_uni;
        *(temhum+1)=Hum_dec*10+Hum_uni;
      }
    }
  	 
  }
  //进程结束
  PROCESS_END();
}
/*******************************************************************/

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
	etimer_set(&tcp_timer, CLOCK_SECOND*600);

  while(1)
  {	    
	//等待定时器时间到
	PROCESS_WAIT_UNTIL(etimer_expired(&tcp_timer));
        //重置定时器
        etimer_reset(&tcp_timer);

	//tcp socket 通信注册
	tcp_socket_register(&s, NULL,
		      inputbuf, sizeof(inputbuf),
		      outputbuf, sizeof(outputbuf),
		      input, event);
	*(buf)='0';
	*(buf+1)='1';
	*(buf+2)=(*(temhum))/10+0x30;
	*(buf+3)=(*(temhum))%10+0x30;
	*(buf+4)='0';
	*(buf+5)='2';
	*(buf+6)=(*(temhum+1))/10+0x30;
	*(buf+7)=(*(temhum+1))%10+0x30;
	*(buf+8)='\0';
	//tcp socket连接服务器
	tcp_socket_connect(&s, &ip6addr, PORT);
	//tcp发送温度湿度数据buf到服务器
	tcp_socket_send_str(&s,(char*)buf);
	printf("%s\r\n",buf);
        tcp_socket_close(&s);
  	 
  }
  //进程结束
  PROCESS_END();
}
/*******************************************************************/
