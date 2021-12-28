#include "contiki.h"                        //contiki系统api库
#include "contiki-net.h"                    //contiki网络api库
#include "contiki-lib.h"                    //contiki系统lib库
#include "lib/random.h"                     //随机数生成库
#include "sys/ctimer.h"                     //系统回调定时器库
#include "sys/etimer.h"                     //系统事件定时器库
#include "net/rpl/rpl.h"                    //路由相关api
#include "net/ip/uip.h"                     //uip网络协议api
#include "net/ipv6/uip-ds6.h"               //ipv6相关api
#include "aes.h"                            //aes库
#include "ip64-addr.h"                      //ip64地址接口库 
#include "apps/mdns/mdns.h"                 //mdns接口
#include "apps/simple-rpl/simple-rpl.h"     //rpl相关
#include "tcp-socket.h"                     //tcp通信协议api
#include "udp-socket.h"                     //udp通信api
#include "http-socket/http-socket.h"        //http socket通信api
#include "cpu.h"                            //CPU库
#include "dev/uart.h"                       //串口驱动库
#include "dev/serial-line.h"                //串口线0处理
#include "dev/serial-line1.h"               //串口线1处理
#include "dev/watchdog.h"                   //看门狗
#include "dev/sys-ctrl.h"
#include "net/rime/broadcast.h"
#include <stdlib.h>                         //标准的系统相关接口
#include <stdio.h>                          //标准设备的输入输出相关接口
#include <string.h>                         //字符串处理库
#include <math.h>
#include "dev/leds.h"                       //LED灯显示驱动
#include "dev/lcd-24.h"			            //2.4寸LCD模块驱动接口
//定义通信端口
#define PORT 8002
//tcp socket 数据结构体
static struct tcp_socket s;
//输入缓冲区为400个字节
#define INPUTBUFSIZE 100
static uint8_t inputbuf[INPUTBUFSIZE];
//输出缓冲区为400个字节
#define OUTPUTBUFSIZE 100
//定义输出缓冲区
static uint8_t outputbuf[OUTPUTBUFSIZE];
//tcp socket 数据对象,用来保存tcp通信相关的数据结构
static struct tcp_socket s;
//ds6配置
static struct uip_ds6_notification n;
//ipv6地址设置
uip_ip6addr_t ip6addr;
//ipv4地址设置
uip_ip4addr_t ip4addr;
static uint8_t tcpsostate=0;     //缓存接收TCP接收状态
static uint8_t RPLrouter =0;     //状态变量，是否侦听到RPL路由 =0，未侦听到
static uint8_t TCPsendRUN =0;    //允许TCP发送，=0，不允许，=0x11允许
static uint8_t StrTCP[90]={0};       //TCP发送帧缓冲区
static uint8_t TcpSumSend=0;          //状态变量，=0发送实参除温度，！=0发送其他
//声明静态共享事件变量
//static process_event_t event_epc_ready;    //EPC读取到
//寻卡命令序列
const uint8_t Inventory[7]={0xBB,0x00,0x22,0x00,0x00,0x22,0x7E};
//选择命令序列帧头
const uint8_t SelectCard[12]={0xBB,0x00,0x0C,0x00,0x13,0x01,0x00,0x00,0x00,0x20,0x60,0x00};
//读存储区块0-3命令序列，存储电量数据、二氧化碳排放量
const uint8_t ReadEnergyBlock[16]={0xBB,0x00,0x39,0x00,0x09,0x00,0x00,0x00,
                             0x00,0x03,0x00,0x00,0x00,0x04,0x49,0x7E};
//写存储区块0-1命令序列帧头，存储电量数据
const uint8_t WriteEnergyBlock[14]={0xBB,0x00,0x49,0x00,0x11,0x00,0x00,0x00,
                             0x00,0x03,0x00,0x00,0x00,0x04};
//读电量参数命令序列
const uint8_t Power[8]={0x01,0x03,0x00,0x48,0x00,0x08,0xC4,0x1A};
//电量清零命令序列
const uint8_t ClearEnergy[13]={0x01,0x10,0x00,0x4B,0x00,0x02,0x04,0x00,0x00,0x00,0x00,0xB6,0x2C};
//标签状态机
static uint8_t CardMachine =0;
//=0,停止状态（有电流进入轮询），=1，轮询状态，
//标签操作状态显示
static uint8_t CardRunState[5]={'>','0','0','>','\0'};
//串口1命令序列发送次数控制
static uint8_t CommandCNT =0;
//比较次数
static uint8_t CompCNT=0;
//定时器倍增计数
static uint8_t TimeCNT=0;
//设备在线
static uint8_t CardONLine=0;
//设备卡号
static uint8_t CardEPC[12]={0};
//设备运行
static uint8_t DeviceRun =0; //状态变量，=0,未运行，=11运行
//标签存储电量值 
static uint8_t CardEnergy[4] ={0};
//标签存储CO2排放量 
static uint8_t CardCO2[4] ={0}; 
//总二氧化碳排放量
static uint8_t AllCO2[4] ={0}; 
//总电量
static uint8_t AllEnergy[4]={0};
//汇总缓存数组
static uint8_t AllArray[4]={0};
//标签电路已读出
static uint8_t CardReadOk =0;   //=0,未读出
//标签首次轮询变量
static uint8_t CardInvBegin =0;   //=11,首次，=55非首次
//电量模块状态机
static uint8_t PowerMachine =0;   //=0,首次上电清零
//电压数据
static uint8_t VoltageData[4]={0};
//电流数据
static uint8_t CurrentData[4]={0};
//有功功率数据
static uint8_t PowerData[4]={0};
//有功总电量
static uint8_t EnergyData[4]={0};
//功率因数
static uint8_t PowerFactor[4]={0};
//二氧化碳排放
static uint8_t CO2Data[4]={0};
//温度
static uint8_t TempterData[4]={0};
//频率
static uint8_t FreqData[4]={0};
//四字节十进制数组
static uint8_t DecData[12]={0};
//四字节十六进制数组数据转换为数据
static uint32_t HEX4Dec(uint8_t *DataBuf)
{
  	uint32_t HexData=0;
  	HexData = (((uint32_t)(*(DataBuf))) <<24);
  	HexData |= (((uint32_t)(*(DataBuf+1))) <<16);
  	HexData |= (((uint32_t)(*(DataBuf+2))) <<8);
  	HexData |= ((uint32_t)(*(DataBuf+3)));
  	return HexData;
}
//四字节数组存储数据求和，返回在数组AllArray
static void ArraySum(uint8_t *Array1,uint8_t *Array2)
{
  	static uint16_t DaBuffer=0;
  	static uint8_t HighData;
  	//第4字节数据，低8位数据
  	DaBuffer =(uint16_t)*(Array1 +3);
  	DaBuffer +=(uint16_t)*(Array2 +3);
  	*(AllArray +3) = (uint8_t)(DaBuffer%256);  //0-7位回送缓存
  	HighData = (uint8_t)(DaBuffer/256);      //进位
  	//第3字节数据
  	DaBuffer =(uint16_t)*(Array1 +2);
  	DaBuffer +=(uint16_t)*(Array2 +2);
  	DaBuffer +=(uint16_t)(HighData);  //加进位
  	*(AllArray +2) = (uint8_t)(DaBuffer%256);  //8-15位回送缓存
  	HighData = (uint8_t)(DaBuffer/256);  //进位
  	//第2字节数据
 	DaBuffer =(uint16_t)*(Array1 +1);
 	DaBuffer +=(uint16_t)*(Array2 +1);
  	DaBuffer +=(uint16_t)(HighData);  //加进位
  	*(AllArray +1) = (uint8_t)(DaBuffer%256);  //16-23位回送缓存
  	HighData = (uint8_t)(DaBuffer/256);  //进位
  	//第1字节数据
  	DaBuffer =(uint16_t)*(Array1);
 	DaBuffer +=(uint16_t)*(Array2);
  	DaBuffer +=(uint16_t)(HighData);  //加进位
  	*(AllArray) = (uint8_t)(DaBuffer%256);  //16-23位回送缓存
}
//四字节十六进制数组数据转换为十进制数组
static void HEXToDecASCII(uint8_t *DataBuf)
{
  	uint32_t HexData=0;
  	HexData = (((uint32_t)(*(DataBuf))) <<24);
  	HexData |= (((uint32_t)(*(DataBuf+1))) <<16);
  	HexData |= (((uint32_t)(*(DataBuf+2))) <<8);
  	HexData |= ((uint32_t)(*(DataBuf+3)));
  	*(DecData) = HexData/1000000000 +0x30;          //9个0
  	*(DecData+1) = HexData%1000000000/100000000 +0x30;    //9/8
  	*(DecData+2) = HexData%100000000/10000000 +0x30;      //8/7
  	*(DecData+3) = HexData%10000000/1000000 +0x30;        //7/6
  	*(DecData+4) = HexData%1000000/100000 +0x30;        //6/5
  	*(DecData+5) = HexData%100000/10000 +0x30;        //5/4
  	*(DecData+6) = HexData%10000/1000 +0x30;        //4/3
  	*(DecData+7) = HexData%1000/100 +0x30;        //3/2
  	*(DecData+8) = HexData%100/10 +0x30;        //2/1
  	*(DecData+9) = HexData%10 +0x30;
  	*(DecData+10) = '\0';   //最后补充字符串结束符
}
//添加小数点，4位小数，4位整数
static void Dec4Dot(void)
{
	static uint8_t tempdata[12]={0};
	//取小数点前6位数据
	memcpy(tempdata,DecData,6);
	*(tempdata+6)= '.';
	memcpy(tempdata+7,DecData+6,4);
	*(tempdata+11)= '\0';
	if(*(tempdata) == '0') {
		*(tempdata) =' ';  //首字符为0，改为空格
		if(*(tempdata+1) == '0') { //第2个字符为0，改为空格
			*(tempdata+1) =' ';
			if(*(tempdata+2) == '0') { //第3个字符为0，改为空格
				*(tempdata+2) =' ';
				if(*(tempdata+3) == '0') { //第4个字符为0，改为空格
					*(tempdata+3) =' ';
					if(*(tempdata+4) == '0') { //第5个字符为0，改为空格
						*(tempdata+4) =' '; 
							}
						}
					}
				}
			}
	memcpy(DecData,tempdata,12);  //转储全局数组
}
//添加小数点，3位小数，1位整数
static void Dec3Dot(void)
{
  	static uint8_t tempdata[12]={0};
  	//取小数点前1位数据
  	*(tempdata+6) = *(DecData+6);
  	*(tempdata+7)= '.';
  	memcpy(tempdata+8,DecData+7,3);
  	*(tempdata+11)= '\0';
  	memcpy(DecData,tempdata,12);  //转储全局数组
}

//添加小数点，2位小数，2位整数
static void Dec2Dot(void)
{
  	static uint8_t tempdata[12]={0};
  	//取小数点前2位数据
  	*(tempdata+6) = *(DecData+6);
  	if(*(tempdata+6) == '0') *(tempdata+6) =' ';  
  	*(tempdata+7) = *(DecData+7);
  	*(tempdata+8)= '.';
  	*(tempdata+9) = *(DecData+8);
  	*(tempdata+10) = *(DecData+9);
  	*(tempdata+11)= '\0';
  	memcpy(DecData,tempdata,12);  //转储全局数组
}
static void HEXToASCII(uint8_t *DataHEX, uint8_t *DataASCII, uint8_t DataLEN)
{
	static uint8_t DataPair[2];
	uint8_t i,j;
	for(i = 0; i < DataLEN; i++){
	*(DataPair) = (*(DataHEX+i) & 0xF0) >> 4;
	*(DataPair+1) = *(DataHEX+i) & 0x0F;
		for (j = 0; j < 2; j++){
			if (*(DataPair+j) < 10){ 			
				*(DataPair+j) += 0x30;
	·				}
			else {
				if (*(DataPair+j) < 16)
				*(DataPair+j) =*(DataPair+j) - 10 + 'A';
			}
			*DataASCII++ = *(DataPair+j);
				}   			// for (int j = ...)
	}  		 	// for (int i = ...)
	*DataASCII ='\0';
}
//计算CRC
static uint16_t CountCRC(uint8_t CrcNum,uint16_t Crctemp) 
{ 
  	uint8_t i; 
  	uint8_t Check; 
  	Crctemp = Crctemp ^ CrcNum;  //字节与CRC寄存器数据异或
  	for(i=0;i<8;i++){ 
    		Check = (uint8_t)(Crctemp & 1);   //取出最低位
    		Crctemp = Crctemp>>1;             //右移1位最高位补0
    		Crctemp = Crctemp & 0x7fff; 
    		if (Check == 1)                   //为1与固定值异或运算
      		Crctemp = Crctemp ^ 0xa001;    
    		Crctemp = Crctemp&0xffff; 
  	} 
  	return Crctemp; 
}
//数据流验证CRC 
static uint16_t CheckCRC(uint8_t *crcbuf,uint8_t datalen)
{ 
  	uint8_t high,low; 
  	uint16_t i; 
  	uint16_t WORDCRC; 
  	WORDCRC=0xFFFF; 
  	for(i=0;i<datalen;i++){ 
    		WORDCRC = CountCRC(*crcbuf,WORDCRC); 
    		crcbuf++; 
  	}
  	high = (uint8_t)(WORDCRC%256); 
  	low =(uint8_t)(WORDCRC/256);
  	WORDCRC=(((uint16_t)(high)) <<8) | low; 
  	return WORDCRC; 
}
//路由回调函数,处理路由事件
static void route_callback(int event, uip_ipaddr_t *route, uip_ipaddr_t *ipaddr,int numroutes)
{
  	if(event == UIP_DS6_NOTIFICATION_DEFRT_ADD) {
    	leds_off(LEDS_ALL);
	RPLrouter =0x11;     //侦听到RPL路由
  	}
}
//TCP socket输入处理函数
static int input(struct tcp_socket *s, void *ptr,const uint8_t *inputptr, int inputdatalen)
{
  	printf("input %d bytes '%s' - closing socket\n", inputdatalen, inputptr);
  	tcp_socket_close(s);
  	return 0;
}
//TCP socket事件函数
static void event(struct tcp_socket *s, void *ptr,tcp_socket_event_t ev)
{
  	if(ev == TCP_SOCKET_CONNECTED) {
      	tcpsostate=1;   //tcp连接
  	} else if(ev == TCP_SOCKET_DATA_SENT) {
    		tcpsostate=2;   //tcp数据发送完成
  	} else if(ev == TCP_SOCKET_CLOSED) {
    		tcpsostate=3;   //socket关闭
  	} else if(ev == TCP_SOCKET_ABORTED) {
    		tcpsostate=4;  //printf("Socket reset\n");
  	} else if(ev == TCP_SOCKET_TIMEDOUT) {
    		tcpsostate =5;  //printf("Socket timedout\n");
  	}
}
//进程声明：TCP处理进程
PROCESS(tcp_link, "tcp_link");
//进程声明：计算进程
PROCESS(caculate_process, "caculate-power");
//进程声明：液晶显示
PROCESS(lcd24_process, "lcd-2.4");
//声明进程：定时发送寻卡
PROCESS(Card_process, "Card-process");
//声明进程：定时读电量参数
PROCESS(Read_Power_process, "Read-Power");
//系统初始化的时候自动启动这个进程
AUTOSTART_PROCESSES(&caculate_process,&lcd24_process,&Card_process,&Read_Power_process,&tcp_link);
//进程实现
PROCESS_THREAD(tcp_link, ev, data)
{
  	//进程开始
  	PROCESS_BEGIN();
  	static uint8_t TcpBuff[90]={0}; 
  	//打开所有led
  	leds_on(LEDS_ALL);
  	//添加回调函数
  	uip_ds6_notification_add(&n, route_callback);
  	//添加ipv6服务器地址
  	uip_ip6addr(&ip6addr, 0x2001,0xda8,0x270,0x2020,0xf816,0x3eff,0xfe88,0xdb99);
  	//创建定时器事件对象
  	static struct etimer ettcp;
  	//设置定时器时间,CLOCK_SECOND时间为1秒钟，10秒钟
  	etimer_set(&ettcp, CLOCK_SECOND*10);
  	while(1){
		//等待定时器事件发生
    		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&ettcp));
    		//重置定时器
    		etimer_reset(&ettcp); 
		memcpy(TcpBuff,StrTCP,26);
		*(TcpBuff +1) ='B';
		memcpy(TcpBuff+26,StrTCP+61,5);   //字符温度值
		memcpy(TcpBuff+31,StrTCP+66,10);   //字符有功总电量值
		memcpy(TcpBuff+41,StrTCP+76,11);   //字符二氧化碳排量
		TcpSumSend = ~TcpSumSend;     //取反状态变量
		if(TCPsendRUN ==0x11) { //允许TCP发送
	  	//tcp socket 通信注册
      		tcp_socket_register(&s, NULL,
                      	inputbuf, sizeof(inputbuf),
                      	outputbuf, sizeof(outputbuf),
                      	input, event);			  
      		PROCESS_PAUSE();
      		//tcp socket连接服务器
      		tcp_socket_connect(&s, &ip6addr, PORT);
	  	PROCESS_PAUSE();
	  	//tcp发送数据帧到服务器
	  	if(TcpSumSend == 0)
	    	tcp_socket_send(&s,StrTCP,61);
	  	else
		tcp_socket_send(&s,TcpBuff,52);
	  	PROCESS_PAUSE();
	  	tcp_socket_close(&s);  //关闭tcp连接
	}
  }
//进程结束
PROCESS_END();
}
//进程实现
PROCESS_THREAD(caculate_process, ev, data)
{ 
  	//进程开始
  	PROCESS_BEGIN();
  	//创建定时器事件对象
  	static struct etimer etcacu;
  	//设置定时器时间,CLOCK_SECOND时间为1秒钟，1秒钟
  	etimer_set(&etcacu, CLOCK_SECOND);
  	while(1)
  	{	
		//等待定时器事件发生
    		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etcacu));
    		//重置定时器
    		etimer_reset(&etcacu);
		*(StrTCP +0) ='D';    //标识设备电量
		*(StrTCP +1) ='A';
		HEXToASCII(CardEPC,StrTCP+2,12); //将EPC数据入TCP发送帧
		HEXToDecASCII(VoltageData);  //电压数据转字符串
		Dec4Dot();    //转换成小数点数组
		memcpy(StrTCP+26,DecData+3,8);   //字符电压值入TCP发送帧
		HEXToDecASCII(CurrentData);  //电流数据转字符串
		Dec4Dot();    //转换成小数点数组
		memcpy(StrTCP+34,DecData+3,8);   //字符电流值入TCP发送帧
		HEXToDecASCII(PowerData);  //有功功率数据转字符串
		Dec4Dot();    //转换成小数点数组
		memcpy(StrTCP+42,DecData+2,9);   //字符有功功率值入TCP发送帧
		HEXToDecASCII(PowerFactor);  //功率因数数据转字符串
		Dec3Dot();  //转换成小数点数组
		memcpy(StrTCP+51,DecData+6,5);   //字符功率因数值入TCP发送帧
		HEXToDecASCII(FreqData);  //频率数据转字符串
		Dec2Dot();  //转换成小数点数组
		memcpy(StrTCP+56,DecData+6,5);   //字符频率值入TCP发送帧
		HEXToDecASCII(TempterData);  //温度数据转字符串
		Dec2Dot();  //转换成小数点数组
		memcpy(StrTCP+61,DecData+6,5);   //字符温度值入TCP发送帧
		ArraySum(EnergyData,CardEnergy);  //电量求和
		memcpy(AllEnergy,AllArray,4);   //转储电量
		HEXToDecASCII(AllEnergy);  //有功总电量数据转字符串
		Dec4Dot();    //转换成小数点数组
		memcpy(StrTCP+66,DecData,10);   //字符有功总电量入TCP发送帧
		ArraySum(CO2Data,CardCO2);  //二氧化碳排量求和
		memcpy(AllCO2,AllArray,4);   //转储二氧化碳排量
		HEXToDecASCII(AllCO2);  //二氧化碳总排量转字符串
		Dec4Dot();    //转换成小数点数组
		memcpy(StrTCP+76,DecData,11);   //字符二氧化碳排量入TCP发送帧
		*(StrTCP +88) ='\0';  //添加数组结束符
		*(StrTCP +89) ='\0';  //添加数组结束符
  	}
//进程结束
PROCESS_END();
}
//进程实现
PROCESS_THREAD(lcd24_process, ev, data)
{ 
  	//进程开始
  	PROCESS_BEGIN();
  	static uint8_t buffString[25]={0};
  	//创建定时器事件对象
  	static struct etimer etdisp;
  	//设置定时器时间,CLOCK_SECOND时间为1秒钟，1秒钟
  	etimer_set(&etdisp, CLOCK_SECOND);
  	while(1)
  	{	
		//等待定时器事件发生
    		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etdisp));
    		//重置定时器
    		etimer_reset(&etdisp);
		/*****************调试显示**********************/
		//路由连接状态显示
		*(CardRunState +0)='[';
		if(RPLrouter == 0)
		*(CardRunState +1) = ' ';
    		else *(CardRunState +1) = '+';
		*(CardRunState +2) = ']';
		*(CardRunState +3) = '\0';
		LCD_ShowString(211,110,CardRunState,BLACK,WHITE,16);
		*(CardRunState +0) = tcpsostate +0x30;      //tcp状态
		*(CardRunState +1) = CardMachine +0x30;    //标签运行状态机
		*(CardRunState +2) = PowerMachine +0x30;   //电量运行状态机
		LCD_ShowString(5,130,CardRunState,BLACK,WHITE,16);
		if(CardONLine ==0){  //没有识别到设备，卡在读取中
	  		LCD_ShowString(40,130,(unsigned char *)"No Device Online        ",DARKBLUE,LGRAY,16);
		}
		else {  //识读到有效设备
      			memcpy(buffString,StrTCP+2,24);   //EPC代码
	  		*(buffString+24)='\0';
	  		LCD_ShowString(40,130,buffString,DARKBLUE,LGRAY,16);   //显示EPC
    		}
    		memcpy(buffString,StrTCP+26,8);   //字符电压值
		*(buffString+8)='\0';
		LCD_ShowString(42,170,buffString,BLACK,YELLOW,16);  //显示电压值
		memcpy(buffString,StrTCP+34,8);   //字符电流值
		*(buffString+8)='\0';
		LCD_ShowString(159,170,buffString,BLACK,YELLOW,16);  //显示电流值	
		memcpy(buffString,StrTCP+42,9);   //字符有功功率值
		*(buffString+9)='\0';
		LCD_ShowString(66,195,buffString,BLACK,YELLOW,16);  //显示有功功率值
		memcpy(buffString,StrTCP+51,5);   //字符功率因数值
		*(buffString+5)='\0';
		LCD_ShowString(191,195,buffString,BLACK,YELLOW,16);  //显示功率因数
		memcpy(buffString,StrTCP+56,5);   //字符频率值
		*(buffString+5)='\0';
		LCD_ShowString(42,220,buffString,BLACK,YELLOW,16);  //显示频率
		memcpy(buffString,StrTCP+61,5);   //字符温度值
		*(buffString+5)='\0';
		LCD_ShowString(175,220,buffString,BLACK,YELLOW,16);  //显示温度
    		memcpy(buffString,StrTCP+66,10);   //字符有功总电量值
		*(buffString+10)='\0';
		LCD_ShowString(16,260,buffString,RED,YELLOW,32);  //显示总电量
		memcpy(buffString,StrTCP+76,11);   //字符有功总电量值
		*(buffString+11)='\0';
		LCD_ShowString(104,297,buffString,BLUE,WHITE,16);
		if(CardReadOk ==0x11){   //已完成读卡
			TCPsendRUN =0x11;    //允许TCP发送
		}
		else {
	  		TCPsendRUN =0;    //禁止TCP发送
		}
  }
//进程结束
PROCESS_END();
}
//Card_process进程实现
PROCESS_THREAD(Card_process, ev, data)
{
  	//进程开始
  	PROCESS_BEGIN();
  	static struct etimer et;
  	uint8_t cnt=0;
  	uint8_t sumcheck=0;
  	static uint8_t EPCBUFF[32];  
  	static uint8_t EPCDATA[36]; //存储3次EPC码
  	//分配事件
  	//event_epc_ready = process_alloc_event();
  	//创建定时器事件对象
  	//设置定时器时间,CLOCK_SECOND时间为1秒钟,1秒
  	etimer_set(&et, CLOCK_SECOND*2);
  	//进程循环
  	while(1)
  	{
  		//等待定时器超时或串口1接收事件
		PROCESS_YIELD();
		if(etimer_expired(&et)) { //定时器到期
	  		if(CardMachine == 1){  //寻卡状态
	    		//寻卡命令序列串口1发送
				if(CardInvBegin ==0x11) {  //首次轮询，获取EPC
	      				if(CommandCNT <3) { //寻卡命令5次
            						for(cnt=0;cnt<7;cnt++)
              						uart_write_byte(1,*(Inventory+cnt));
		   	 			CommandCNT ++;   //命令次数加
	      				}
				}
				else {  //10秒定时轮询，检查EPC
		  			if(CardInvBegin ==0x55) { //首次轮询结束
		    				TimeCNT++;    //定时器10倍增
            						if(TimeCNT == 10){ //10秒检查1次
			  				TimeCNT =0;     //等待下一1秒
			  				CommandCNT ++;   //命令次数加  	
			  				for(cnt=0;cnt<7;cnt++)
                						uart_write_byte(1,*(Inventory+cnt));
						}
		  			}
				}
	  		}
	  		else {  //状态机为2
        				if(CardMachine == 2){ //选择状态
		  			if(CommandCNT < 5) {  //选择命令最多送5次
		    				memcpy(EPCBUFF,SelectCard,12);    //选择命令数据头入列
		    				memcpy(EPCBUFF+12,CardEPC,12);    //EPC号入列
		    				sumcheck =0;   //校验和清零
		    				for(cnt=1;cnt<24;cnt++) {  //计算校验和
		      				sumcheck += *(EPCBUFF+cnt);   
		    			}
		    		*(EPCBUFF+24) = sumcheck;    //校验和入列
		    		*(EPCBUFF+25) = 0x7E;        //帧结束入列
		    		for(cnt=0;cnt<26;cnt++)
              				uart_write_byte(1,*(EPCBUFF+cnt));
		    		CommandCNT ++;   //命令次数加
		  	}
          			else {  //发送选择命令序列超5次未反应
            				//重新轮询EPC
				DeviceRun = 0;     //设备未运行
		    		CardONLine =0;     //置标签未在线
		    		CardMachine =0;    //禁止读卡
		    		memset(CardEPC,0,12);  //EPC清零   
		  	}			  
        		}
        		else {  //状态机为3
          			if(CardMachine == 3){ //读标签存储电量数据块
		    		for(cnt=0;cnt<16;cnt++)
              				uart_write_byte(1,*(ReadEnergyBlock+cnt));   //读电量存储数据
		  	}
		  	else {  //状态机为4
		    		if(CardMachine == 4) {  //写标签存储电量数据块
			  		memcpy(EPCBUFF,WriteEnergyBlock,14);    //写命令帧数据头入列
			  		memcpy(EPCBUFF+14,AllEnergy,4);           //待写电量数据入列
			  		memcpy(EPCBUFF+18,AllCO2,4);           //待写总二氧化碳排量数据入列
			  		sumcheck =0;   //校验和清零
		      			for(cnt=1;cnt<22;cnt++) {  //计算校验和
		        			sumcheck += *(EPCBUFF+cnt);   
		      		}
		      		*(EPCBUFF+22) = sumcheck;    //校验和入列
		      		*(EPCBUFF+23) = 0x7E;        //帧结束入列
		      		for(cnt=0;cnt<24;cnt++)
                			uart_write_byte(1,*(EPCBUFF+cnt));  //发送命令帧
		   	}
            			else { //其他状态 
			}
		  }
		}			
	  }
	  etimer_restart(&et);
	}
	else if(ev== serial_line1_event_message)  { //串口1接收事件
	  //提取数据到缓冲区
	  memcpy(EPCBUFF,data,*((uint8_t *)(data)));
 	  if(CardMachine == 1){ //寻卡状态
	    if(CardInvBegin ==0x11) {  //首次轮询，获取EPC
		  //提取EPC代码
          if((*(EPCBUFF+1) == 2) && (*(EPCBUFF+2)== 0x22) ){//判断寻卡数据正常
  	        memcpy(EPCDATA+CompCNT*12,EPCBUFF+8,12);
		    CompCNT ++;   //存储次数加
	      }
	      else { //未读到正确EPC
	    
	      }
		  if(CommandCNT ==3) {//寻卡结束
		    if(CompCNT >0) {
			  memcpy(CardEPC,EPCDATA+(CompCNT-1)*12,12);  //取最新一次标签号到全局变量
              CardONLine =0x11;   //表示识读到EPC，设备置在线
			  CardMachine =2;     //进入选择读卡流程
		    }
		    else {  //一个EPC也未读到，重新按上电
			  CardONLine =0; 
			  DeviceRun = 0;     //设备未运行
		      CardONLine =0;     //置标签未在线
		      CardMachine =0;    //禁止读卡
		      memset(CardEPC,0,12);  //EPC清零   
		    }
			CommandCNT =0;  //清除变量，备其他状态使用
			CompCNT =0;    //清除变量，备其他状态使用
		    memset(EPCDATA,0,36);  //清除数据缓冲区
		  }
		}
		else { //非首次轮询
		  if(CardInvBegin ==0x55) {
		    if((*(EPCBUFF+1) == 2) && (*(EPCBUFF+2)== 0x22) ){//判断寻卡数据正常
			  if(memcmp(CardEPC,EPCBUFF+8,12) ==0) CompCNT ++;   //正确次数加
	        }
	        else { //未读到正确EPC
	    
	        }
			if(CommandCNT ==6) {//6次应答，1分钟到写标签
		      if(CompCNT >0) {   //识读到标签EPC
			    CardONLine =0x11;   //标签在线表示识读到EPC，设备置在线
                CardMachine =2;     //进入选择写卡流程
		      }
		      else {  //一个EPC也未读到
			    CardONLine =0;   //标签离线 
				CardInvBegin =0x11;  //重新首次轮询
				CardMachine =1;     
		      }
			  CommandCNT =0;  //清除变量，备其他状态使用
			  CompCNT =0;    //清除变量，备其他状态使用
		      TimeCNT =0;    //清除变量，备其他状态使用
		    }
		  }
		}
	  }   //状态机1
	  else {  //状态机为2
	    if(CardMachine == 2) {
		  //查看响应帧
          if((*(EPCBUFF+1) == 1) && (*(EPCBUFF+2)== 0x0C) && (*(EPCBUFF+5)== 0)){//判断响应帧正常
  	        //选择成功进入允许读写状态
			if(CardInvBegin ==0x11) {  //首次轮询，获取到EPC
			  CardMachine =3;  //读电量数据
			}
			else {
			  if(CardInvBegin ==0x55) {  //正常巡检
			    CardMachine =4;  //写电量数据到标签
			  }
			}
			CommandCNT =0;    //次数变量清零
		  }
	      else { //未收到正确响应帧
	      }
		}
		else {  //状态机为3
		  if(CardMachine == 3) {  //读标签存储电量数据块
            //查看读数据响应帧
            if((*(EPCBUFF+1) == 1) && (*(EPCBUFF+2)== 0x39) && (memcmp(CardEPC,EPCBUFF+8,12)== 0)){//判断读响应帧正常
  	          memcpy(CardEnergy,EPCBUFF+20,4);   //卡存储电量数据到内存
			  memcpy(CardCO2,EPCBUFF+24,4);   //卡存储二氧化碳排量数据到内存
			  if(CardReadOk ==0){  //首次读卡
			    CardReadOk =0x11;    //已读出
				CardInvBegin =0x55;  //已读出卡存储电量
				CardMachine =1;     //返回轮询
			  }
			  else {  }
		    }
	        else {
              if((*(EPCBUFF+1) == 1) && (*(EPCBUFF+2)== 0xFF) && (*(EPCBUFF+5)== 0x09)){//标签不在场区或EPC不对
		        CardMachine =2;  //重新选择发送
			  }
			  else {   //其他响应帧
			  }	  
	        }  
		  }
		  else {  //状态机为4
            if(CardMachine == 4) {  //写标签存储电量数据块
              //查看写数据响应帧
              if((*(EPCBUFF+1) == 1) && (*(EPCBUFF+2)== 0x49) && (*(EPCBUFF+20)== 0) && (memcmp(CardEPC,EPCBUFF+8,12)== 0)){//判断写响应帧正常
  	            //写数据成功
				CardMachine = 1;   //回到轮询状态
		      }
	          else {
                if((*(EPCBUFF+1) == 1) && (*(EPCBUFF+2)== 0xFF) && (*(EPCBUFF+5)== 0x10)){//标签不在场区或EPC不对
			      CardMachine =2;  //重新选择发送
			    }
			    else {   //其他响应帧
			    }
              }				
			}
            else { //其他状态
			
		    }
		  }
		}
	  }
	}
  }
  //进程结束
  PROCESS_END();
}
//Read_Power_process进程实现
PROCESS_THREAD(Read_Power_process, ev, data)
{
  //进程开始
  PROCESS_BEGIN();
  static struct etimer ett;
  uint8_t cntt=0;
  uint8_t Temp;
  static uint8_t PwrBuff[40];
  static uint16_t CrcData; 
  //创建定时器事件对象
  //设置定时器时间,CLOCK_SECOND时间为1秒钟,1秒
  etimer_set(&ett, CLOCK_SECOND);
  //进程循环
  while(1)
  {
    //等待定时器超时或串口0接收事件
	PROCESS_YIELD();
	if(etimer_expired(&ett)) { //定时器到期
	  if(PowerMachine == 0){ //写寄存器
	    //发送电量清零命令序列，串口0发送
        for(cntt=0;cntt<13;cntt++)
          uart_write_byte(0,*(ClearEnergy+cntt));
	  }
	  else { //状态机1
	    if(PowerMachine == 1){ //读电量
	      //发送读取电量参数命令序列，串口0发送
          for(cntt=0;cntt<8;cntt++)
            uart_write_byte(0,*(Power+cntt));
	    }
        else {  //其他状态
		}			
	  }
	  etimer_restart(&ett);  //重启定时器
	}
	else if(ev== serial_line_event_message) { 
	//串口0接收事件
	  //提取数据帧到缓冲区
	  memcpy(PwrBuff,data,*((uint8_t *)(data)));
      Temp = *PwrBuff; //缓存数据长度
	  *PwrBuff = 0x01;    //修改为帧头
	  CrcData = CheckCRC(PwrBuff,Temp-2);  //CRC计算
	  if((*(PwrBuff+Temp-2) == (uint8_t)(CrcData/256)) && (*(PwrBuff+Temp-1) == (uint8_t)(CrcData%256))) {
	    if(PowerMachine == 1) { //状态机为1
		  if((*(PwrBuff+1) == 0x03) && (*(PwrBuff+2) == 0x20)) { //回答帧为电量数据
		    memcpy(VoltageData,PwrBuff+3,4);  //缓存电压数据
	        memcpy(CurrentData,PwrBuff+7,4);  //缓存电流数据+3+4
		    if(HEX4Dec(CurrentData) != 0) { //查看电流测量是否为0，没有启动设备
	          if(DeviceRun ==0) {  //首次读到电流
		        DeviceRun = 0x11;     //设备运行
				CardInvBegin =0x11;   //置位首次轮询
		        CardMachine =1;    //可以开始读卡
		      }
			  else { //正常运行态读到电流
			  }
	        }
		    else {  //读到电流为零，不需处理
            }
		    memcpy(PowerData,PwrBuff+11,4);  //缓存有功功率数据+3+4+4
		    memcpy(EnergyData,PwrBuff+15,4);  //缓存有功总电量数据+3+4+4+4
            memcpy(PowerFactor,PwrBuff+19,4);  //缓存功率因数数据+3+4+4+4+4
            memcpy(CO2Data,PwrBuff+23,4);  //缓存二氧化碳排放数据+3+4+4+4+4+4
            memcpy(TempterData,PwrBuff+27,4);  //缓存温度数据+3+4+4+4+4+4+4
            memcpy(FreqData,PwrBuff+31,4);  //缓存频率数据+3+4+4+4+4+4+4+4
	      } else { }  //其他回答帧，不需处理
		}
		else {   //状态机0
		  if(PowerMachine == 0){
		    //电量清零，写寄存器
		    if((*(PwrBuff+1) == 0x10) && (*(PwrBuff+3) == 0x4B)&& (*(PwrBuff+5) == 0x02)) { //回答帧为写数据正常
		      PowerMachine = 1;    //修改状态机为1，读电量状态
		    }
		    else { PowerMachine = 1; }//其他数据，可能写不成功，不需处理
		  }
		  else { //其他状态
		  }
		}
	  }
      else {  //校验错误处理
	  }
	}
  }
  //进程结束
  PROCESS_END();
}