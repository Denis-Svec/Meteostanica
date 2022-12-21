/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "i2c.h"
#include "gpio.h"
#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "stdio.h"
#include "string.h"
#include "dma.h"
#include "hts221.h"
#include "lps22hb.h"
#include <math.h>
#include "ili9163.h"
#include "spi.h"
#include "predictw.h"

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

char formated_temp[50];
char formated_pressure[50];
char formated_forecast[50];
int last_forecast = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void DrawSun(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t rx_data = 0;
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

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */
uint8_t state;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  lsm6ds0_init();
  hts221_init();
  lps22hb_init();
  lis3mdl_init();


  initCD_Pin();
   initCS_Pin();
   initRES_Pin();

   LL_mDelay(50);

   lcdInitialise(LCD_ORIENTATION3);
   lcdClearDisplay(decodeRgbValue(0, 0, 0));





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {	  // setup & info

	  state = weather();

	  memset(formated_temp, '\0', sizeof(formated_temp));
	  memset(formated_pressure, '\0', sizeof(formated_pressure));
	  memset(formated_forecast, '\0', sizeof(formated_forecast));
	  sprintf(formated_temp, "Temp [C]: %0.1f", hts221_get_temp());
	  sprintf(formated_pressure, "Press [Pa]: %0.1f", lps22hb_get_pressure());

	  LL_mDelay(200);

	  lcdPutS(formated_temp, lcdTextX(2), lcdTextY(3), decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
	  lcdPutS(formated_pressure, lcdTextX(2), lcdTextY(3)+10, decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));



	  // forecast
	  if((state==1) && (last_forecast != 1))
	  {
		  lcdClearDisplay(decodeRgbValue(0, 0, 0));
		  LL_mDelay(1000);
		  DrawSunny();
		  sprintf(formated_forecast, "SUNNY");
		  lcdPutS(formated_forecast, lcdTextX(2)+40, lcdTextY(3)+25, decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
		  last_forecast = 1;
	  }

	  if((state==2)&& (last_forecast != 2))
	  	  {
		  lcdClearDisplay(decodeRgbValue(0, 0, 0));
		  LL_mDelay(1000);
		  DrawCloudySunny();
		  sprintf(formated_forecast, "PARTIALLY CLOUDY");
		  lcdPutS(formated_forecast, lcdTextX(2)+8, lcdTextY(3)+25, decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
		  last_forecast = 2;
	  	  }

	  if((state==3)&& (last_forecast != 3))
	  	  {
	  		lcdClearDisplay(decodeRgbValue(0, 0, 0));
	  		LL_mDelay(1000);
	  		DrawCloudy();
	  		sprintf(formated_forecast, "CLOUDY");
	  		lcdPutS(formated_forecast, lcdTextX(2)+36, lcdTextY(3)+25, decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
	  	  	last_forecast = 3;
	  	  	  }

	  if((state==4)&& (last_forecast != 4))
	  	  {
	  		lcdClearDisplay(decodeRgbValue(0, 0, 0));
	  		LL_mDelay(1000);
		  	DrawWorsening();
		  	sprintf(formated_forecast, "Worsening");
		  	lcdPutS(formated_forecast, lcdTextX(2)+28, lcdTextY(3)+25, decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
		  	last_forecast = 4;
	  	  	  	  }

	  if((state==5)&& (last_forecast != 5) && (hts221_get_temp() >= 0) )
	 // if((state==5)&& (last_forecast != 5)&& (temp2 >= 0))
	  	  {
	  		lcdClearDisplay(decodeRgbValue(0, 0, 0));
	  		LL_mDelay(1000);
	  	  	DrawRaining();
	  	  	sprintf(formated_forecast, "Raining");
	  	  	lcdPutS(formated_forecast, lcdTextX(2)+35, lcdTextY(3)+25, decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
	  	  	last_forecast = 5;
	  	  	 }

	  if((state==5)&& (last_forecast != 5)&& (hts221_get_temp() < 0))
	//  if((state==5)&& (last_forecast != 5)&& (temp2 < 0))
	  	  	  {
	  	  		lcdClearDisplay(decodeRgbValue(0, 0, 0));
	  	  		LL_mDelay(1000);
	  	  	  	DrawSnow();
	  	  	  	sprintf(formated_forecast, "Snowing");
	  	  	  	lcdPutS(formated_forecast, lcdTextX(2)+35, lcdTextY(3)+25, decodeRgbValue(255, 255, 255), decodeRgbValue(0, 0, 0));
	  	  	  	last_forecast = 5;
	  	  	  	 }




	  LL_mDelay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */
void DrawSunny(void){

	for(int i=12; i>0;i--){
		lcdCircle(64, 85, i, decodeRgbValue(255, 255, 51));
	}

	lcdLine(64, 65, 64, 105, decodeRgbValue(255, 255, 51));
	lcdLine(42, 85, 85, 85, decodeRgbValue(255, 255, 51));
	lcdLine(50, 70, 79, 99, decodeRgbValue(255, 255, 51));
	lcdLine(49, 100, 80, 70, decodeRgbValue(255, 255, 51));
}

void DrawCloudySunny(void){
	//sun
	for(int i=12; i>0;i--){
			lcdCircle(64, 85, i, decodeRgbValue(255, 255, 51));
		}

	for(int i=10; i>0;i--){
					lcdCircle(54, 100, i, decodeRgbValue(255, 255, 51));
					lcdCircle(74, 100, i, decodeRgbValue(255, 255, 51));
	}

	for(int i=8; i>0;i--){

				lcdCircle(64, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(44, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(84, 105, i, decodeRgbValue(255, 255, 255));
			}



		lcdLine(64, 65, 64, 82, decodeRgbValue(255, 255, 255));
		lcdLine(42, 85, 85, 85, decodeRgbValue(255, 255, 255));
		lcdLine(50, 70, 79, 99, decodeRgbValue(255, 255, 255));
		lcdLine(49, 100, 80, 70, decodeRgbValue(255, 255, 255));
}


void DrawCloudy(void){

	for(int i=10; i>0;i--){
					lcdCircle(54, 100, i, decodeRgbValue(255, 255, 255));
					lcdCircle(74, 100, i, decodeRgbValue(255, 255, 255));
	}

	for(int i=8; i>0;i--){

				lcdCircle(64, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(44, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(84, 105, i, decodeRgbValue(255, 255, 255));
			}
}

void DrawWorsening(void){

	for(int i=10; i>0;i--){
					lcdCircle(54, 100, i, decodeRgbValue(255, 255, 255));
					lcdCircle(74, 100, i, decodeRgbValue(255, 255, 255));
					lcdCircle(94, 100, i, decodeRgbValue(255, 255, 255));
					lcdCircle(34, 100, i, decodeRgbValue(255, 255, 255));
	}

	for(int i=8; i>0;i--){

				lcdCircle(64, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(44, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(84, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(104, 105, i, decodeRgbValue(255, 255, 255));
				lcdCircle(24, 105, i, decodeRgbValue(255, 255, 255));
			}
}

void DrawRaining(void){

	for(int i=10; i>0;i--){ // big circles
						lcdCircle(54, 100, i, decodeRgbValue(255, 255, 255));
						lcdCircle(74, 100, i, decodeRgbValue(255, 255, 255));
						lcdCircle(94, 100, i, decodeRgbValue(255, 255, 255));
						lcdCircle(34, 100, i, decodeRgbValue(255, 255, 255));
		}

		for(int i=8; i>0;i--){ // small circles

					lcdCircle(64, 105, i, decodeRgbValue(255, 255, 255));
					lcdCircle(44, 105, i, decodeRgbValue(255, 255, 255));
					lcdCircle(84, 105, i, decodeRgbValue(255, 255, 255));
					lcdCircle(104, 105, i, decodeRgbValue(255, 255, 255));
					lcdCircle(24, 105, i, decodeRgbValue(255, 255, 255));
				}

		// rain
		lcdLine(24, 118, 24, 124, decodeRgbValue(0, 0, 255));
		lcdLine(34, 114, 34, 122, decodeRgbValue(0, 0, 255));
		lcdLine(44, 118, 44, 124, decodeRgbValue(0, 0, 255));
		lcdLine(54, 114, 54, 122, decodeRgbValue(0, 0, 255));
		lcdLine(64, 118, 64, 124, decodeRgbValue(0, 0, 255));
		lcdLine(74, 114, 74, 122, decodeRgbValue(0, 0, 255));
		lcdLine(84, 118, 84, 124, decodeRgbValue(0, 0, 255));
		lcdLine(94, 114, 94, 122, decodeRgbValue(0, 0, 255));
		lcdLine(104, 118, 104, 124, decodeRgbValue(0, 0, 255));



	}

void DrawSnow(void){

	lcdCircle(64, 85, 12, decodeRgbValue(0, 0, 255));
	lcdCircle(64, 85, 7, decodeRgbValue(0, 0, 255));
	lcdCircle(64, 85, 2, decodeRgbValue(0, 0, 255));

	lcdLine(64, 65, 64, 105, decodeRgbValue(0, 0, 255));
	lcdLine(42, 85, 85, 85, decodeRgbValue(0, 0, 255));
	lcdLine(50, 70, 79, 99, decodeRgbValue(0, 0, 255));
	lcdLine(49, 100, 80, 70, decodeRgbValue(0, 0, 255));
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
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
