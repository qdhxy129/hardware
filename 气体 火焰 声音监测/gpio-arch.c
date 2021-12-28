#include "dev/gpio.h"   //gpio定义
#include "clock.h"      //时钟库
#include "gpio-arch.h"
void GPIO_ARCH_init()
{ 
  	GPIO_SET_INPUT(YW_PORT,YW_BIT);
  	GPIO_SET_INPUT(HY_PORT,HY_BIT);
  	GPIO_SET_INPUT(MQ7_PORT,MQ7_BIT);
  	GPIO_SET_OUTPUT(JDQ_PORT,JDQ_BIT);
}
unsigned int fire,fume,CO;
int HY_Status()
{
  	GPIO_ARCH_init();
  	if(GPIO_READ_PIN(HY_PORT,HY_BIT)==1)
   	{
		GPIO_WRITE_PIN(JDQ_PORT,JDQ_BIT,JDQ_BIT);
      		printf("FIRE!!\n");
       		fire=1;
      		return fire;
   	}
   	else
	{
    		GPIO_WRITE_PIN(JDQ_PORT,JDQ_BIT,0);
      		printf("HY Normal\n");
         		fire=0;
       		return fire;
	}
}
 int YW_Status()
{
    	GPIO_ARCH_init();
   	if(GPIO_READ_PIN(YW_PORT,YW_BIT)==0)
  	{
      		printf("FUME!!\n");
         		fume=1;
		return fume;
   	}
   	else
	{
      		printf("YW Normal\n");
        		fume=0;
		return fume;
	}
}
int MQ7_Status()
{
   	GPIO_ARCH_init();
   	if(GPIO_READ_PIN(MQ7_PORT,MQ7_BIT)==0)
   	{
      		printf("CO!!\n");
         		CO=1;
		return CO;
   	}
   	else
	{
      		printf("CO Normal\n");
        		CO=0;
		return CO;
	}
}