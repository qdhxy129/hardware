#ifndef __GPIO_ARCH_H__
#define	__GPIO_ARCH_H__
//PC0
#define HY_PORT   GPIO_C_BASE
#define HY_BIT    (1 << 0) 
//PC1
#define YW_PORT   GPIO_C_BASE
#define YW_BIT        2
//PD0
#define JDQ_PORT   GPIO_D_BASE
#define JDQ_BIT    (1 << 0)
//PC2
#define MQ7_PORT   GPIO_D_BASE
#define MQ7_BIT        8
extern  int HY_Status();
extern  int YW_Status();
extern   int MQ7_Status();
extern  unsigned int fire,fume,CO;
#endif
