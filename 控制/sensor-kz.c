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
#include <stdlib.h>                         //标准的系统相关接口
#include <stdio.h>                          //标准设备的输入输出相关接口
#include <string.h>                         //字符串处理库
//#include "dev/kz.h"                       //LED驱动库
/*------------------------------------------------------------*/
/******************全局定义区**************************************/
//定义通信端口
#define PORT 8002
//tcp socket 数据结构体
static struct tcp_socket s;
static struct tcp_socket s1;
//输入缓冲区为400个字节
#define INPUTBUFSIZE 400
static uint8_t inputbuf[INPUTBUFSIZE];
static uint8_t inputbuf1[INPUTBUFSIZE];
//输出缓冲区为400个字节
#define OUTPUTBUFSIZE 400
//定义输出缓冲区
static uint8_t outputbuf[OUTPUTBUFSIZE];
static uint8_t outputbuf1[OUTPUTBUFSIZE];

static struct uip_ds6_notification n;
static struct uip_ds6_notification n1;

/***********全局静态变量*****************************************/
        //定义变量
	static unsigned int FLAG=0;
	static unsigned char txbuf[8]="QQ\0";
	static unsigned char txbuf1[8]="KZ\0";
	unsigned char rxbuf[17]="HELLO-+QQ+-WORLD\0";
	unsigned char rxbuf1[17]="HELLO-+KZ+-WORLD\0";
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
  if(event == UIP_DS6_NOTIFICATION_DEFRT_ADD)
  {
    leds_off(LEDS_ALL);
   // printf("Got a RPL route\n");
  }
}
//路由回调函数,处理路由事件
static void
route_callback1(int eventt, uip_ipaddr_t *route, uip_ipaddr_t *ipaddr,
               int numroutes)
{
  if(eventt == UIP_DS6_NOTIFICATION_DEFRT_ADD)
  {
    leds_off(LEDS_ALL);
   // printf("Got a RPL route\n");
  }
}


/*---------------------------------------------------------------------------*/

//输入处理函数
static int
input(struct tcp_socket *s, void *ptr,
      const uint8_t *inputptr, int inputdatalen)
{
	  memcpy(rxbuf,inputptr,16);

 return 0;
}
//输入处理函数1
static int
input1(struct tcp_socket *s1, void *ptr1,
      const uint8_t *inputptr1, int inputdatalen1)
{
	  memcpy(rxbuf1,inputptr1,16);

 return 0;
}

	 


//事件函数
static void
event(struct tcp_socket *s, void *ptr,tcp_socket_event_t ev)
{
  if(ev == TCP_SOCKET_CONNECTED) {
   // printf("Socket connected\n");
  } else if(ev == TCP_SOCKET_DATA_SENT) {
   // printf("Socket data was sent\n");
  } else if(ev == TCP_SOCKET_CLOSED) {
  //  printf("Socket closed\n");
  } else if(ev == TCP_SOCKET_ABORTED) {
   // printf("Socket reset\n");
  } else if(ev == TCP_SOCKET_TIMEDOUT) {
   // printf("Socket timedout\n");
  }
}

//事件函数
static void
eventt(struct tcp_socket *s1, void *ptr1,tcp_socket_event_t ev1)
{
  if(ev1 == TCP_SOCKET_CONNECTED) {
   // printf("Socket connected\n");
  } else if(ev1 == TCP_SOCKET_DATA_SENT) {
   // printf("Socket data was sent\n");
  } else if(ev1 == TCP_SOCKET_CLOSED) {
  //  printf("Socket closed\n");
  } else if(ev1 == TCP_SOCKET_ABORTED) {
   // printf("Socket reset\n");
  } else if(ev1 == TCP_SOCKET_TIMEDOUT) {
   // printf("Socket timedout\n");
  }
}


/******************************************************************/
//进程声明
PROCESS(tcp_link, "tcp_link");
PROCESS(tcp_link1, "tcp_link1");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&tcp_link,&tcp_link1);

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
	etimer_set(&tcp_timer, CLOCK_SECOND*2);

  while(1)
	{	    
	
	//等待定时器时间到
	PROCESS_WAIT_UNTIL(etimer_expired(&tcp_timer));
        //重置定时器
        etimer_reset(&tcp_timer);
if(FLAG==0)
{
	//tcp socket 通信注册
	tcp_socket_register(&s, NULL,
		      inputbuf, sizeof(inputbuf),
		      outputbuf, sizeof(outputbuf),
		      input, event);


	//tcp socket连接服务器
	tcp_socket_connect(&s, &ip6addr, PORT);

	//tcp发送请求到服务器
	tcp_socket_send_str(&s,(char *)txbuf);	
	
       // tcp_socket_close(&s);
 	printf("%s\r\n",rxbuf);
	if(*rxbuf==(char *)'D' && *(rxbuf+1)==(char *)'D')
{
	leds_on(LEDS_ORANGE);	
}
	FLAG=1;
}
	}


  //进程结束
  PROCESS_END();
}
/************************************************/

//tcp进程实现
PROCESS_THREAD(tcp_link1, ev1, data)
{ 
	//进程开始
	PROCESS_BEGIN();
  	//添加ipv6服务器地址
  	uip_ip6addr(&ip6addr, 0x2001,0xda8,0x270,0x2020,0xf816,0x3eff,0xfe88,0xdb99);
	//打开所有led
	leds_on(LEDS_ALL);
	//添加回调函数
	uip_ds6_notification_add(&n1, route_callback1);
	// 定时器
	static struct etimer tcp_timer1;
	//定时器时间设置
	etimer_set(&tcp_timer1, CLOCK_SECOND*2);

  while(1)
	{	    
	
	//等待定时器时间到
	PROCESS_WAIT_UNTIL(etimer_expired(&tcp_timer1));
        //重置定时器
        etimer_reset(&tcp_timer1);

if(FLAG==1)
{
	//tcp socket 通信注册1
	tcp_socket_register(&s1, NULL,
		      inputbuf1, sizeof(inputbuf1),
		      outputbuf1, sizeof(outputbuf1),
		      input1, eventt);


	//tcp socket连接服务器
	tcp_socket_connect(&s1, &ip6addr, PORT);
	//tcp发送请求到服务器
	tcp_socket_send_str(&s1,(char *)txbuf1);	
       // tcp_socket_close(&s);
	printf("%s\r\n",rxbuf1);
 	if(*rxbuf1==(char *)'K' && *(rxbuf1+1)==(char *)'Z')
{
leds_on(LEDS_GREEN);	
}
		
FLAG=0;
	}

}
  //进程结束
  PROCESS_END();
}

