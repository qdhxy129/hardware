/*
 * Copyright (c) 2012, Texas Instruments Incorporated - http://www.ti.com/
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * \addtogroup cc2538-platforms
 * @{
 *
 * \defgroup cc2538dk The cc2538 Development Kit platform
 *
 * The cc2538DK is a platform by Texas Instruments, based on the
 * cc2538 SoC with an ARM Cortex-M3 core.
 * @{
 *
 * \file
 *   Main module for the cc2538dk platform
 */
/*---------------------------------------------------------------------------*/
#include "contiki.h"            //系统函数
#include "dev/adc.h"            //ADC驱动
#include "dev/leds.h"           //LED驱动

#include "dev/sys-ctrl.h"
#include "dev/scb.h"
#include "dev/nvic.h"
#include "dev/uart.h"           //串口驱动
#include "dev/watchdog.h"       //看门狗驱动
#include "dev/ioc.h"            //ioc驱动
#include "dev/lcd-24.h"			//2.4寸LCD模块驱动接口


#include "dev/serial-line.h"    //串口线0驱动
#include "dev/serial-line1.h"   //串口线1驱动
#include "dev/slip.h"
#include "dev/cc2538-rf.h"      //射频驱动
#include "dev/udma.h"           //串口dma驱动
#include "usb/usb-serial.h"     //usb虚拟串口驱动
#include "lib/random.h"         //随机数生成函数
#include "net/netstack.h"       //网络协议栈
#include "net/queuebuf.h"       //
#include "net/ip/tcpip.h"       //tcpip接口
#include "net/ip/uip.h"         //uip接口
#include "net/mac/frame802154.h"//802.15.4协议接口
#include "cpu.h"                //CPU相关
#include "reg.h"                //寄存器相关
#include "ieee-addr.h"
#include "lpm.h"

#include "net/ip64/ip64.h"      //ip64转换相关      
#include "../apps/aes/netstack-aes.h"//ase相关
#include "../apps/simple-rpl/simple-rpl.h"//rpl相关

#include <stdint.h>             //标准的系统数据类型定义相关库
#include <string.h>             //字符串处理库
#include <stdio.h>              //标准输入输出库

/*---------------------------------------------------------------------------*/
//启动版本配置
#if STARTUP_CONF_VERBOSE
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif
//串口使能
#if UART_CONF_ENABLE
#define PUTS(s) puts(s)
#else
#define PUTS(s)
#endif
/*---------------------------------------------------------------------------*/
//LED闪烁
static void
fade(unsigned char l)
{
  volatile int i;
  int k, j;
  for(k = 0; k < 800; ++k) {
    j = k > 400 ? 800 - k : k;

    leds_on(l);
    for(i = 0; i < j; ++i) {
      asm("nop");
    }
    leds_off(l);
    for(i = 0; i < 400 - j; ++i) {
      asm("nop");
    }
  }
}

/*---------------------------------------------------------------------------*/
//RF通信参数配置
static void
set_rf_params(void)
{
  uint16_t short_addr;
  uint8_t ext_addr[8];

  ieee_addr_cpy_to(ext_addr, 8);

  short_addr = ext_addr[7];
  short_addr |= ext_addr[6] << 8;

  /* Populate linkaddr_node_addr. Maintain endianness */
  //内存拷贝函数
  /*
  原型：extern void *memcpy(void *dest, void *src, unsigned int count);
  用法：#include <string.h>
  功能：由src所指内存区域复制count个字节到dest所指内存区域。
  说明：src和dest所指内存区域不能重叠，函数返回指向dest的指针。
  */

  memcpy(&linkaddr_node_addr, &ext_addr[8 - LINKADDR_SIZE], LINKADDR_SIZE);

#if STARTUP_CONF_VERBOSE
  {
    int i;
    printf("Rime configured with address ");
    for(i = 0; i < LINKADDR_SIZE - 1; i++) {
      printf("%02x:", linkaddr_node_addr.u8[i]);
    }
    printf("%02x\n", linkaddr_node_addr.u8[i]);
  }
#endif
  //网络参数配置
  NETSTACK_RADIO.set_value(RADIO_PARAM_PAN_ID, IEEE802154_PANID); //PAN_ID
  NETSTACK_RADIO.set_value(RADIO_PARAM_16BIT_ADDR, short_addr);   //地址
  NETSTACK_RADIO.set_value(RADIO_PARAM_CHANNEL, CC2538_RF_CHANNEL);//通道
  NETSTACK_RADIO.set_object(RADIO_PARAM_64BIT_ADDR, ext_addr, 8);//外部地址
}
/*---------------------------------------------------------------------------*/
/**
 * \brief Main routine for the cc2538dk platform
 */
/*
CC2538DK平台主函数

*/ 
int
main(void)
{
  nvic_init();                //中断优先级初始化
  ioc_init();
  sys_ctrl_init();            //系统控制寄存器初始化
  clock_init();               //时钟初始化
  lpm_init();
  rtimer_init();              //定时器初始化
  gpio_init();                //gpio初始化

  leds_init();                //LED灯初始化
  //黄灯闪烁
  fade(LEDS_YELLOW);    

  process_init();             //进程初始化

  watchdog_init();            //看门狗初始化
 
  //LCD模块初始化
  LCD_Init();
  //LCD清屏：填充黑色                  
  LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
  DispScreen((unsigned char *)"    0",(unsigned char *)"    0",(unsigned char *)"    0");
  /*
   * Character I/O Initialisation.
   * When the UART receives a character it will call serial_line_input_byte to
   * notify the core. The same applies for the USB driver.
   *
   * If slip-arch is also linked in afterwards (e.g. if we are a border router)
   * it will overwrite one of the two peripheral input callbacks. Characters
   * received over the relevant peripheral will be handled by
   * slip_input_byte instead
   */
//串口使能
#if UART_CONF_ENABLE
  //初始化串口0、串口1
  uart_init(0);
  uart_init(1);
 
  //uart_set_input(SERIAL_LINE_CONF_UART, serial_line_input_byte);
  uart_set_input(0, serial_line_input_byte);
  uart_set_input(1, serial_line1_input_byte);
#endif
//USB虚拟串口使能
#if USB_SERIAL_CONF_ENABLE
  usb_serial_init();                            //usb虚拟串口初始化
  usb_serial_set_input(serial_line_input_byte); //配置usb虚拟串口输入类型
#endif
  //line类型串口初始化
  serial_line_init();
  serial_line1_init();
  //中断使能
  INTERRUPTS_ENABLE();
  //绿色LED闪烁
  fade(LEDS_GREEN);
  //输出contiki版本信息
  PUTS(CONTIKI_VERSION_STRING);
  //输出开发板信息
  PUTS(BOARD_STRING);

  PRINTF(" Net: ");
  PRINTF("%s\n", NETSTACK_NETWORK.name);
  PRINTF(" MAC: ");
  PRINTF("%s\n", NETSTACK_MAC.name);
  PRINTF(" RDC: ");
  PRINTF("%s\n", NETSTACK_RDC.name);

  /* Initialise the H/W RNG engine. */
  //随机数生成器初始化
  random_init(0);
  //udma初始化
  udma_init();
  //启动进程
  process_start(&etimer_process, NULL);
  //定时器初始化
  ctimer_init();
  //设置RF参数
  set_rf_params();
  //网络协议栈初始化
  netstack_init();
//IPV6网络协议栈配置
#if NETSTACK_CONF_WITH_IPV6
  memcpy(&uip_lladdr.addr, &linkaddr_node_addr, sizeof(uip_lladdr.addr));
  queuebuf_init();
  //启动tcpip进程
  process_start(&tcpip_process, NULL);
#endif /* NETSTACK_CONF_WITH_IPV6 */
  //adc初始化
  adc_init();
 
  //myself
  //低功耗模块初始化
  energest_init();
  //设置低功耗模块的CPU类型
  ENERGEST_ON(ENERGEST_TYPE_CPU);
  //路由功能初始化
#if ROUTER
  simple_rpl_init();
  ip64_init();
#endif /*ROUTER*/
//关闭RDC网络层
  NETSTACK_RDC.off(1);

  {
//网络的AES_KEY设置
#ifndef NETSTACK_AES_KEY
#define NETSTACK_AES_KEY "thingsquare mist"
#define NETSTACK_AES_KEY_DEFAULT 1
#endif /* NETSTACK_AES_KEY */
//输出警告信息到编译器的消息窗口,当前的编译器不会停止编译
//#if NETSTACK_AES_KEY_DEFAULT
//#warning Using default AES key "thingsquare mist", change it in project-conf.h like this:
//#warning #define NETSTACK_AES_KEY {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}
//#endif /* NETSTACK_AES_KEY */
    //网络的AES_KEY
    const uint8_t key[] = NETSTACK_AES_KEY;
    //输出设置ASE key
    printf("Setting AES key\n");
    //设置网络ase key
    netstack_aes_set_key(key);
  }
  //自动启动进程
  autostart_start(autostart_processes);
  //启动看门狗定时器
  watchdog_start();
  //橙色LED闪烁
  fade(LEDS_ORANGE);
  //主循环
  while(1) {
    uint8_t r;
    do {
      /* Reset watchdog and handle polls and events */
      //复位看门狗,处理轮询和事件
      watchdog_periodic();
      //处理系统所有needspoll标记为1的进程及处理事件队列的下一个事件
      r = process_run();
    } while(r > 0);

    /* We have serviced all pending events. Enter a Low-Power mode. */
    //进入低功耗模式
    lpm_enter();
  }
}
/*---------------------------------------------------------------------------*/

/**
 * @}
 * @}
 */
