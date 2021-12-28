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
#define LCD_SCK_PORT  GPIO_B_BASE
#define LCD_SCK_BIT   (1 << 0)
#define LCD_SCK       LCD_SCK_PORT, LCD_SCK_BIT
//SDA=MOSI
#define LCD_MOSI_PORT GPIO_B_BASE
#define LCD_MOSI_BIT  (1 << 1)
#define LCD_MOSI      LCD_MOSI_PORT, LCD_MOSI_BIT

//RES
#define LCD_RST_PORT   GPIO_B_BASE
#define LCD_RST_BIT    (1 << 2)
#define LCD_RST        LCD_RST_PORT, LCD_RST_BIT

//DC
#define LCD_DC_PORT   GPIO_B_BASE
#define LCD_DC_BIT    (1 << 3)
#define LCD_DC        LCD_DC_PORT, LCD_DC_BIT
//CS
#define LCD_CS_PORT   GPIO_B_BASE
#define LCD_CS_BIT    (1 << 4)
#define LCD_CS        LCD_CS_PORT, LCD_CS_BIT



void delay_us(unsigned int time);
void delay_ms(unsigned int time);
void LCD_GPIO_Config( void );

void LCD_Writ_Bus(unsigned char dat);//模拟SPI时序
void LCD_WR_DATA8(unsigned char dat);//写入一个字节
void LCD_WR_DATA(unsigned int dat);//写入两个字节
void LCD_WR_REG(unsigned char dat);//写入一个指令
void LCD_Address_Set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);//设置坐标函数
void LCD_Init(void);//LCD初始化

void LCD_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);//指定区域填充颜色
void LCD_DrawPoint(unsigned int x,unsigned int y,unsigned int color);//在指定位置画一个点
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);//在指定位置画一条线
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);//在指定位置画一个矩形
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color);//在指定位置画一个圆

void LCD_ShowChinese(unsigned int x,unsigned int y,unsigned char len,unsigned char *index,unsigned int fc,unsigned int bc,unsigned char sizey);//显示汉字串
void LCD_ShowChinese16x16(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index);//显示单个16x16汉字
void LCD_ShowChinese24x24(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index);//显示单个24x24汉字
void LCD_ShowChinese32x32(unsigned int x,unsigned int y,unsigned int fc,unsigned int bc,unsigned char index);//显示单个32x32汉字

void LCD_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey);//显示一个字符
void LCD_ShowString(unsigned int x,unsigned int y,const unsigned char *p,unsigned int fc,unsigned int bc,unsigned char sizey);//显示字符串
unsigned long mypow(unsigned char m,unsigned char n);//求幂
void LCD_ShowIntNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//显示整数变量
void LCD_ShowFloatNum1(unsigned int x,unsigned int y,float num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);//显示两位小数变量

void LCD_ShowPicture(unsigned int x,unsigned int y,unsigned int length,unsigned int width,const unsigned char pic[]);//显示图片
void DispScreen(unsigned char *dataEPC,unsigned char *dataV,unsigned char *dataA,unsigned char *dataP,unsigned char *dataCOS,
                   unsigned char *dataHZ,unsigned char *dataOC,unsigned char *dataKWH,unsigned char *dataCO2);
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0   //
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F    //紫红
#define GREEN         	 0x07E0    //绿色
#define CYAN          	 0x7FFF    //青色
#define YELLOW        	 0xFFE0      //黄色
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)    

#endif
