#ifndef __LCD_24_H__
#define __LCD_24_H__
#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320
#else
#define LCD_W 320
#define LCD_H 240
#endif		 
//端口配置定义
//SCL=SCLK
#define LCD_SCLK_PORT  GPIO_B_BASE
#define LCD_SCLK_BIT   (1 << 0)
#define LCD_SCLK       LCD_SCLK_PORT, LCD_SCLK_BIT
//SDA=MOSI
#define LCD_MOSI_PORT GPIO_B_BASE
#define LCD_MOSI_BIT  (1 << 1)
#define LCD_MOSI      LCD_MOSI_PORT, LCD_MOSI_BIT
//RES
#define LCD_RES_PORT   GPIO_B_BASE
#define LCD_RES_BIT     (1 << 2)
#define LCD_RES        LCD_RES_PORT, LCD_RES_BIT
//DC
#define LCD_DC_PORT   GPIO_B_BASE
#define LCD_DC_BIT       (1 << 3)
#define LCD_DC        LCD_DC_PORT, LCD_DC_BIT
//CS
#define LCD_CS_PORT   GPIO_B_BASE
#define LCD_CS_BIT        (1 << 4)
#define LCD_CS        LCD_CS_PORT, LCD_CS_BIT
//生成相关数组
unsigned char hzin[6];
unsigned char *lcdbuff;
void delay_us(unsigned int time);
void delay_ms(unsigned int time);
void LCD_GPIO_Config(void);
void LCD_Writ_Bus(unsigned char dat);//模拟SPI时序
void LCD_WR_DATA1(unsigned char dat);//写入一个字节
void LCD_WR_DATA2(unsigned int dat);//写入两个字节
void LCD_WR_REG(unsigned char dat);//写入一个指令
void LCD_Addr_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);//指定区域填充颜色
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);//在指定位置画一个点
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);//在指定位置画一条线
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);//在指定位置画一个矩形
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color);//在指定位置画一个圆
void LCD_DisplayChinese(unsigned int x,unsigned int y,unsigned char len,unsigned char *index,unsigned int fc,unsigned int bc,unsigned char sizey);//显示汉字串
void LCD_DisplayChinese16x16(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index);//显示单个16x16汉字
void LCD_DisplayChinese24x24(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index);//显示单个24x24汉字
void LCD_DisplayChinese32x32(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index);//显示单个32x32汉字
void LCD_DisplayChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey);//显示一个字符
void LCD_DisplayString(unsigned int x,unsigned int y,const unsigned char *p,unsigned int fc,unsigned int bc,unsigned char sizey);//显示字符串
unsigned long mypow(unsigned char m,unsigned char n);//求幂
void LCD_DisplayIntNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//显示整数变量
void LCD_DisplayFloatNum1(unsigned int x,unsigned int y,float num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//显示两位小数变量
void LCD_DisplayPicture(unsigned int x,unsigned int y,unsigned int length,unsigned int width,const unsigned char pic[]);//显示图片
void DisplayScreen(unsigned char *lux2,unsigned char *eco22,unsigned char *tvoc2);//固定显示
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED		 0xF81F
#define GRED 		 0XFFE0 
#define GBLUE	     	 0X07FF
#define RED           	                 0xF800
#define MAGENTA       	 0xF81F    
#define GREEN         	 0x07E0    
#define CYAN          	 0x7FFF    
#define YELLOW        	 0xFFE0      
#define BROWN 	                 0xBC40 
#define BRRED 		 0xFC07
#define GRAY  	                 0x8430 
#define DARKBLUE      	 0x01CF	
#define LIGHTBLUE      	 0x7D7C	
#define GRAYBLUE       	 0x5458 
#define LIGHTGREEN     	 0x841F 
#define LGRAY 	                 0xC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE                 0xA651 //浅灰蓝色(中间层颜色)
#define LBBLU		 0X2B12 //浅棕蓝色(选择条目的反色)    
#endif