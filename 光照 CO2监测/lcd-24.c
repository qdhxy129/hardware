#include "reg.h"		//寄存器定义
#include "dev/lcd-24.h"	//液晶屏库
#include "dev/gpio.h"	//gpio库
#include "clock.h"		//时钟库
#include "dev/font.h"	//字库
#include "dev/pic.h"	//图片库
//us延时
void delay_us(unsigned int time)
{
  	clock_delay_usec(time);
}
//ms延时
void delay_ms(unsigned int time)
{ 
  	unsigned int i;
  	for(i=0;i<time;i++)
  	clock_delay_usec(1000);
}
//gpio设置
static void
GPIO_Set(int port, int bit)
{
  	GPIO_WRITE_PIN(port, bit, bit);
}
//gpio复位
static void
GPIO_Reset(int port, int bit)
{
  	GPIO_WRITE_PIN(port, bit, 0);
}
//LCD接口配置
void LCD_GPIO_Config( void )
{
  	GPIO_SET_OUTPUT(LCD_CS_PORT,LCD_CS_BIT);
  	GPIO_SET_OUTPUT(LCD_MOSI_PORT,LCD_MOSI_BIT);
  	GPIO_SET_OUTPUT(LCD_SCK_PORT,LCD_SCK_BIT);
  	GPIO_SET_OUTPUT(LCD_DC_PORT,LCD_DC_BIT);
  	GPIO_SET_OUTPUT(LCD_RST_PORT,LCD_RST_BIT);
}
//-----------------LCD函数重封装--------------- 
#define LCD_SCLK_Clr() GPIO_Reset(LCD_SCK_PORT,LCD_SCK_BIT)
#define LCD_SCLK_Set() GPIO_Set(LCD_SCK_PORT,LCD_SCK_BIT)
#define LCD_MOSI_Clr() GPIO_Reset(LCD_MOSI_PORT,LCD_MOSI_BIT)
#define LCD_MOSI_Set() GPIO_Set(LCD_MOSI_PORT,LCD_MOSI_BIT)
#define LCD_RES_Clr()  GPIO_Reset(LCD_RST_PORT,LCD_RST_BIT)
#define LCD_RES_Set()  GPIO_Set(LCD_RST_PORT,LCD_RST_BIT)
#define LCD_DC_Clr()   GPIO_Reset(LCD_DC_PORT,LCD_DC_BIT)
#define LCD_DC_Set()   GPIO_Set(LCD_DC_PORT,LCD_DC_BIT)	     
#define LCD_CS_Clr()   GPIO_Reset(LCD_CS_PORT,LCD_CS_BIT)
#define LCD_CS_Set()   GPIO_Set(LCD_CS_PORT,LCD_CS_BIT)
/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(unsigned char dat) 
{	
	unsigned char i;
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
  LCD_CS_Set();	
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(unsigned char dat)
{
	LCD_Writ_Bus(dat);
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(unsigned int dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}
/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(unsigned char dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//写数据
}
/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
	LCD_WR_REG(0x2a);//列地址设置
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b);//行地址设置
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c);//储存器写
}
void LCD_Init(void)
{
	LCD_GPIO_Config();//初始化GPIO
	LCD_RES_Clr();//复位
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);           
                delay_ms(100);
	LCD_WR_REG(0x11);  
	delay_ms(120);             
	LCD_WR_REG(0xCF);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xC1);
	LCD_WR_DATA8(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x64);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0X12);
	LCD_WR_DATA8(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x85);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x79);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA8(0x39);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA8(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x1D);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0x12);
	LCD_WR_REG(0xC5);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3F);
	LCD_WR_REG(0xC7);
	LCD_WR_DATA8(0x92);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x55);
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x12);
	LCD_WR_REG(0xB6);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0xA2);
	LCD_WR_REG(0x44);
	LCD_WR_DATA8(0x02);
	LCD_WR_REG(0xF2);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x26);
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x1C);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x48);
	LCD_WR_DATA8(0xB8);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0XE1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x47);
	LCD_WR_DATA8(0x4B);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x0F);
	LCD_WR_REG(0x29); //Display on
} 
/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
      xend,yend   终止坐标
      color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{          
	unsigned int i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}
/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
} 
/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
	unsigned int t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}
/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}
/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
	 len 显示汉字长度
                *index 要显示的汉字串的位置数组
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
      返回值：  无
******************************************************************************/
void LCD_DisplayChinese(unsigned int x,unsigned int y,unsigned char len,unsigned char *index,unsigned int fc,unsigned int bc,unsigned char sizey)
{
	unsigned char i;
	for(i=0;i<len;i++)
	{
		if(sizey==16) LCD_ShowChinese16x16(x+i*sizey,y,fc,bc,index[i]);
		else if(sizey==24) LCD_ShowChinese24x24(x+i*sizey,y,fc,bc,index[i]);
		else if(sizey==32) LCD_ShowChinese32x32(x+i*sizey,y,fc,bc,index[i]);
		else LCD_ShowChinese32x32(x+i*sizey,y,fc,bc,index[i]);
	}
}
/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                fc 字的颜色
                bc 字的背景色
                index 显示汉字在相应字库中的位置           
      返回值：  无
******************************************************************************/
void LCD_DisplayChinese16x16(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index)
{
	unsigned char i,j;
	LCD_Address_Set(x,y,x+15,y+15);
	for(i=0;i<32;i++)
	{
		for(j=0;j<8;j++)
		{	
			if(tfont16[index][i]&(0x01<<j))LCD_WR_DATA(fc);
					else LCD_WR_DATA(bc);
		}
	}
} 
/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                fc 字的颜色
                bc 字的背景色
                index 显示汉字在相应字库中的位置           
      返回值：  无
******************************************************************************/
void LCD_DisplayChinese24x24(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index)
{
	unsigned char i,j;
	LCD_Address_Set(x,y,x+23,y+23);
	for(i=0;i<72;i++)
	{
		for(j=0;j<8;j++)
		{	
			if(tfont24[index][i]&(0x01<<j))LCD_WR_DATA(fc);
					else LCD_WR_DATA(bc);
		}
	}
} 
/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                fc 字的颜色
                bc 字的背景色
                index 显示汉字在相应字库中的位置           
      返回值：  无
******************************************************************************/
void LCD_DisplayChinese32x32(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index)
{
	unsigned char i,j;
	LCD_Address_Set(x,y,x+31,y+31);
	for(i=0;i<128;i++)
	{
		for(j=0;j<8;j++)
		{	
			if(tfont32[index][i]&(0x01<<j))LCD_WR_DATA(fc);
					else LCD_WR_DATA(bc);
		}
	}
}
/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号                
      返回值：  无
******************************************************************************/
void LCD_DisplayChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey)
{
	unsigned char temp,sizex,t;
	unsigned char i,TypefaceNum;//一个字符所占字节大小
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else temp=ascii_3216[num][i];		 //调用16x32字体
		for(t=0;t<8;t++)
		{
			if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
		}
	}   	 	  
}
/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
       返回值：  无
******************************************************************************/
void LCD_DisplayString(unsigned int x,unsigned int y,const unsigned char *p,unsigned int fc,unsigned int bc,unsigned char sizey)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey);
		x+=sizey/2;
		p++;
	}  
}
/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
unsigned long mypow(unsigned char m,unsigned char n)
{
	unsigned long result=1;	 
	while(n--)result*=m;
	return result;
}
/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_DisplayIntNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;
	unsigned char sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey);
	}
} 
/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_DisplayNum1(unsigned int x,unsigned int y,float num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey)
{         	
	unsigned char t,temp,sizex;
	unsigned int num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey);
	}
}
/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LCD_DisplayPicture(unsigned int x,unsigned int y,unsigned int length,unsigned int width,const unsigned char pic[])
{
	unsigned int i,j;
	unsigned long k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}
/******************************************************************************
	函数说明:固定显示
	入口数据:lux2 光照值
		 eco22 CO2值
		 tvoc2 有机物值
	返回值	:无
*******************************************************************************/
void DisplayScreen(unsigned char *lux2,unsigned char *eco22,unsigned char *tvoc2)
{
  	//插入图片
  	LCD_ShowPicture(0,0,240,320,gImage_1);
  	//光照组
  	hzin[0]=6;hzin[1]=7;
  	LCD_ShowChinese(170,140,2,hzin,YELLOW,BLACK,32);
  	//CO2组
  	lcdbuff=(unsigned char *)"ppm";
  	LCD_ShowString(170,190,lcdbuff,YELLOW,BLACK,32);
  	//有机物组
  	lcdbuff=(unsigned char *)"ppm";
  	LCD_ShowString(170,250,lcdbuff,YELLOW,BLACK,32);
  	//光照值
  	lcdbuff=(unsigned char *)lux2;
  	LCD_ShowString(90,140,lcdbuff,YELLOW,BLACK,32);
  	//CO2值
  	lcdbuff=(unsigned char *)eco22;
  	LCD_ShowString(90,190,lcdbuff,YELLOW,BLACK,32);
  	//有机物值
  	lcdbuff=(unsigned char *)tvoc2;
  	LCD_ShowString(90,250,lcdbuff,YELLOW,BLACK,32);
}