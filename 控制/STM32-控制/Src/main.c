/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t buff_red[128]={0x64,0x10,0x00,0x00,0x00,0x06,0x0C,0x00,0x01,0x00,0x04,0x00,0x0F,0x00,0X0F,0x00,0x0F,0x00,0x0F,0xB5,0x29};
uint8_t buff_green[128]={0x64,0x10,0x00,0x00,0x00,0x06,0x0C,0x00,0x01,0x00,0x04,0x00,0xF0,0x00,0XF0,0x00,0xF0,0x00,0xF0,0xDE,0x42};
uint8_t buff_blue[128]={0x64,0x10,0x00,0x00,0x00,0x06,0x0C,0x00,0x01,0x00,0x04,0x0F,0x00,0x0F,0X00,0x0F,0x00,0x0F,0x00,0x28,0x74};
uint8_t buff_buzz[128]={0x64,0x10,0x00,0x1E,0x00,0x05,0x0A,0x00,0x01,0x0B,0xB8,0x00,0x0F,0x00,0XC8,0X01,0XF4,0X85,0X69};
uint8_t buff_all_off[128]={0X64,0X10,0X00,0X00,0X00,0X01,0X02,0X00,0X00,0X31,0X02};
 uint8_t Flag=0;
 uint8_t flag_pwm=0;//CHANNEL1标记 门
 uint8_t flag_pwm1=0;//CHANNEL2标记 窗
 uint8_t flag_cur=0; //窗帘标记
 uint8_t flag_tem=0;
 uint8_t flag_hum=0;
 uint8_t flag_co2=0;
 uint8_t flag_ill=0;
 uint8_t flag_peo=0;
 uint8_t flag_yan=0;
 uint8_t flag_fire=0;
 uint8_t flag_co=0;
 uint8_t flag_mq135=0;
 uint8_t flag_sound=0;
 uint8_t flag_pts=0;
 uint8_t flag_elec=0;
static uint8_t flag_btn_light=0;
static uint8_t flag_btn_fan=0;
static uint8_t flag_btn_window=0;
static uint8_t flag_btn_door=0;
static uint8_t flag_btn_cur=0;
uint8_t buff[20];
uint8_t usart4RxBuf[20];
uint8_t i;
uint8_t ReceiveOK=0;
uint8_t RxCount=0;
uint8_t RxBuffer;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
	
	HAL_UART_Transmit(&huart1,(uint8_t *)"hello word",10,0XFFFF);
	
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);//开启信道
  
	HAL_UART_Receive_IT(&huart4,&RxBuffer,1); //uart4接受字符
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		if(ReceiveOK==1)
		{
			ReceiveOK=0;
HAL_UART_Transmit(&huart1,buff,18,0XFFFF);
			Flag=0;
			
			if(buff[0]=='D' && buff[1]=='D' && buff[14]=='S' && buff[15]=='P' )  //校验头尾
			{
				Flag=1;
			}	
			else if(buff[0]=='K' && buff[1]=='Z' && buff[14]=='S' && buff[15]=='P' )  //校验头尾
			{
				Flag=2;
			}	
		
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
			if (Flag==1)
		{			
				/************数据处理****************/
			
				if(buff[2]=='1') //温度  
				{
					flag_tem=1;
				}
					else
					{
					flag_tem=0;
					}			
				if(buff[3]=='1') //湿度
				{
					flag_hum=1;
				}
					else
					{
					flag_hum=0;
					}			
					if(buff[4]=='1') //co2
				{
					
					flag_co2=1;
				}
					else
					{
					flag_co2=0;
					}			
					if(buff[5]=='1') //光照
				{
					flag_ill=1;
				}
					else
					{
					flag_ill=0;
					}			
					if(buff[6]=='1') //人体
				{
					flag_peo=1;
				}
					else
					{
					flag_peo=0;
					}			
					if(buff[7]=='1') //烟雾
				{
					flag_yan=1;
				}
					else
					{
					flag_yan=0;
					}			
					if(buff[8]=='1') //火焰控制报警灯，拉高pe6，开启串口二485发送，控制报警灯
				{
					flag_fire=1;
				}
					else
					{
					flag_fire=0;
					}			
					if(buff[9]=='1') //CO
				{
					flag_co=1;
				}
					else
					{
					flag_co=0;
					}			
					if(buff[10]=='1') //mq135有毒气体
				{
					flag_mq135=1;
				}
					else
					{
					flag_mq135=0;
					}			
					if(buff[11]=='1') //声音
				{
					flag_sound=1;
				}
					else
					{
					flag_sound=0;
					}			
					if(buff[12]=='1') //光敏
				{
					flag_pts=1;
				}
					else
					{
					flag_pts=0;
					}			
					if(buff[13]=='1')//电量
				{
					flag_elec=1;
				}
					else
					{
					flag_elec=0;
					}	
							  for(int i=0;i<18;i++)//清空数组
								{
									buff[i]=0;
								}
				Flag=0; 
		}
		
		if(Flag==2)
		{
					//灯
					if(buff[2]=='K' && buff[3]=='D')
				{
					 flag_btn_light=1;
				}
					else if(buff[2]=='G' && buff[3]=='D')
					{
							flag_btn_light=0;
					}	
			
					//开门
						if(buff[4]=='K' && buff[5]=='M')
				{
							flag_btn_door=1;
							
				}
					else if(buff[4]=='G' && buff[5]=='M')
					{
							flag_btn_door=0;
						
					}	
			//风扇
						if(buff[6]=='K' && buff[7]=='F' && buff[8]=='S')
				{
						flag_btn_fan=1;  
				}
					else if(buff[6]=='G' && buff[7]=='F' && buff[8]=='S')
					{
							flag_btn_fan=0;   
					}	
					//窗帘
						if(buff[9]=='K' && buff[10]=='C' && buff[11]=='L')
				{
						flag_btn_cur=1;  
				}
					else if(buff[9]=='G' && buff[10]=='C' && buff[11]=='L')
					{
							flag_btn_cur=0;   
					}	
					//窗 channel 2
						if(buff[12]=='K' && buff[13]=='C')
				{
							flag_btn_window=1;  
				}
					else if(buff[12]=='G' && buff[13]=='C')
					{
							flag_btn_window=0;  
					}	
					
												  for(int i=0;i<18;i++)//清空数组
													{
														buff[i]=0;
													}
					Flag=0;
		}
		
  }
		
	//app按键处理
	//1.灯 pe0
	if(flag_btn_light==1)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);	 //顶灯1
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);	 //顶灯2
	}
	else if(flag_btn_light==0)
					{
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);	 //顶灯1
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET);	 //顶灯2
		}
		//1.灯，服务器下发数据控制
		//人体，声音，光敏 有人为1 声音为1，光照有光为1
	//	if(flag_peo==1 || flag_sound==1 || flag_ill==0)//如果有人||有声音||无光
		if(flag_peo==1 || flag_sound==1)//如果有人||有声音
		{
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_RESET);		//楼道灯
			HAL_Delay(5000);
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0,GPIO_PIN_SET);	
		}
	

					
	//2.风扇 ，按键控制 pe1
			if(flag_btn_fan==1)
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);		
	}
	else if(flag_btn_fan==0)
					{
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);	
		}
	//2.风扇下发数据控制
		
			if(flag_tem==1 || flag_yan==1 )//如果温度太高或者烟雾很大
		{
			HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_RESET);		
		}

	
	//3.窗户.app按键控制
				if(flag_pwm1==0 && flag_btn_window==1)
	{
	 	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,15);  //转90度--开窗
			HAL_Delay(500);
			flag_pwm1=1;	
	}
		else if(flag_pwm1==1 && flag_btn_window==0)
						{
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,5);  //关
				HAL_Delay(500);
				flag_pwm1=0;		
			}
	  //3.下发数据控制
			if(flag_pwm1==0 && (flag_co2 || flag_co || flag_mq135))
			{
				 	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,15);  //转90度--开窗
			HAL_Delay(500);
			flag_pwm1=1;	
			}

			//4.门app控制
			if(flag_pwm==0 && flag_btn_door==1)
			{
				__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,15);  //90度
							HAL_Delay(500);
							flag_pwm=1;
			}
			else if(flag_pwm==1 && flag_btn_door==0)
				{
							__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,5);  //0度
							HAL_Delay(500);
							flag_pwm=0;
					}
				
				//5.app控制窗帘
				if(flag_cur==0 && flag_btn_cur==1)
				{
					HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);	//电机1 PE2   开
					HAL_Delay(1000);
					HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);
					flag_cur=1;
				}
				else if(flag_cur==1 && flag_btn_cur==0)
					{
					HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);	//电机2  PE3   关
						HAL_Delay(1000);
						HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);
					flag_cur=0;
				}
				//	6.报警灯控制   当着火，或者检测到有毒气体时
				if(flag_fire==1 || flag_mq135==1)
				
				{
					//拉高pe6，开启串口二485发送，控制报警灯	
					HAL_GPIO_WritePin(GPIOE,GPIO_PIN_6,GPIO_PIN_SET);
					//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
					HAL_UART_Transmit(&huart2,buff_red,21,0xffff);
					HAL_Delay(500);
					HAL_UART_Transmit(&huart2,buff_buzz,19,0xffff);
					HAL_Delay(500);
					HAL_UART_Transmit(&huart2,buff_green,21,0xffff);
					HAL_Delay(500);
					HAL_UART_Transmit(&huart2,buff_blue,21,0xffff);
					HAL_Delay(500);
				}
				else
				{
					HAL_UART_Transmit(&huart2,buff_all_off,11,0xffff);// 关闭
					HAL_Delay(500);
				}
					
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 if(huart->Instance==UART4)  //判断是哪个串口产生的中断
 {
  usart4RxBuf[RxCount]=RxBuffer;
  RxCount++;
  if(RxCount==18)
  { 
   for(i=0;i<18;i++)
   {
    buff[i]=usart4RxBuf[i];
   } 
   ReceiveOK=1;    //置位标志
   RxCount=0;
  }
  HAL_UART_Receive_IT(&huart4,&RxBuffer,1);
 }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
