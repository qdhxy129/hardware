#include "contiki.h"            //contiki系统接口
#include "sys/clock.h"          //系统时钟
#include "dev/ioc.h"            //ioc驱动
#include "dev/gpio.h"           //gpio驱动
#include "dev/adc.h"            //adc驱动
#include <stdio.h>
#include <stdint.h>             //标准数据类型定义库
#include "adc-arch.h"
#include "dev/soc-adc.h"
void adc_yw_init()
{
	GPIO_SOFTWARE_CONTROL(GPIO_A_BASE, ADC_MQ135_OUT_PIN_MASK);
	GPIO_SET_INPUT(GPIO_A_BASE, ADC_MQ135_OUT_PIN_MASK);
	ioc_set_over(GPIO_A_NUM, ADC_MQ135_OUT_PIN, IOC_OVERRIDE_ANA);
	GPIO_SOFTWARE_CONTROL(GPIO_A_BASE, ADC_MQ7_OUT_PIN_MASK);
	GPIO_SET_INPUT(GPIO_A_BASE, ADC_MQ7_OUT_PIN_MASK);
	ioc_set_over(GPIO_A_NUM, ADC_MQ7_OUT_PIN, IOC_OVERRIDE_ANA);
}


