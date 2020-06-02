/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "atarist.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 HID_USBDevicesTypeDef* usb;
 HID_KEYBD_Info_TypeDef *k_pinfo;
 HAL_StatusTypeDef status;
 uint8_t RxBuffer[5] = {0};
 uint8_t RxPrev = 0;

 //keyboard,joystick and mouse state
 MouseStateTypeDef MouseState;
 JoystickStateTypeDef JoystickState;


 uint8_t mouse_YAtTop = 1;
 uint8_t mouse_YAtBotton = 0;




 //joystick data
 uint8_t  joydata1 = 0;
 uint8_t  joydata2 = 0;

 uint8_t joydata1_prev = 0;
 uint8_t joydata2_prev = 0;

 //mouse data
 MouseStatusTypeDef MouseStatus;
















/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int __io_putchar(int ch)
{
 uint8_t c[1];
 c[0] = ch & 0x00FF;
 HAL_UART_Transmit(&huart3, &*c, 1, 5);
 return ch;
}

int _write(int file,char *ptr, int len)
{
 int DataIdx;
 for(DataIdx= 0; DataIdx< len; DataIdx++)
 {
 __io_putchar(*ptr++);
 }
return len;
}


uint8_t MapJoystick(uint8_t joystick_data)
{

	  uint8_t joymapped = 0x00;

	   	//Map any button to
		   	//BTN0 = (joydata1>>7&0x1);
		   	//BTN1 =  (joydata1>>6&0x1);
		    //BTN2 =  (joydata1>>5&0x1);
		   	//BTN3 =  (joydata1>>4&0x1);


		   	//fire
		   	if((joystick_data>>7&0x1)||(joystick_data>>6&0x1)||(joystick_data>>5&0x1))
		   			{
		   				joymapped = joymapped|0x80;
		   			}
			//mask 0x01 up
		   	//mask 0x02 down
			//mask 0x04
			//mask 0x08




		   	//UP = (*joymap>>3&0x1); Map one of buttons as a jump.
		   	if((joystick_data>>3&0x1)||(joystick_data>>4&0x1))
		   	{
		   		joymapped = joymapped|0x01;
		   	}

		   	//DOWN = (*joymap>>2&0x1);
		 	if(joystick_data>>2&0x1)
			{
		 		joymapped = joymapped|0x02;
			}

			//LEFT = (*joydata1>>1&0x1);
			 	if(joystick_data>>1&0x1)
			 			{
			 				joymapped = joymapped|0x04;
			 			}

		 	////RIGHT = (*joydata1&0x1);

		 	if(joystick_data&0x1)
		 			{
		 				joymapped = joymapped|0x08;
		 			}
		 	return joymapped;
}


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

   //Set defaults
   MouseStatus.mouse_threshold_x = 1;
   MouseStatus.mouse_threshold_y = 1;

   MouseStatus.absolute_mouse_max_x = 0xFFFF;
   MouseStatus.absolute_mouse_max_y = 0xFFFF;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_HOST_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  initKeyboard(&huart2);

  HAL_UART_Receive_IT(&huart2, RxBuffer, 1);


  //Ack start
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
  printf("!!STM32ToAtari started!! \r\n");





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    MX_USB_HOST_Process();
    usb = USBH_HID_GetUSBDev();


    //Keyboard data
    if(usb->keyboard!=NULL)
    {
   	 processKbd(usb->keyboard);
    }





//Relative position reporting
 /*
  %111110xy         ; mouse position record flag
                    ; where y is the right button state
                    ; and x is the left button state
X                   ; delta x as twos complement integer
Y                   ; delta y as twos complement integer


*/
 if (usb!=NULL&&usb->mouse!=NULL)
 {
	 if (MouseState==MOUSE_RELATIVE &&  JoystickState!= JOYSTICK_INTERROGATION_MODE)
	 {
		     uint8_t mouse[3] = {0};
			 mouse[0] = 0xF8;
		 	 mouse[0]= mouse[0]|(usb->mouse->buttons[0]<<1);
		 	 mouse[0]= mouse[0]|(usb->mouse->buttons[1]);
		 	 if (abs(usb->mouse->x)>=(MouseStatus.mouse_threshold_x)) mouse[1] = usb->mouse->x;
		 	 if (abs(usb->mouse->y)>=(MouseStatus.mouse_threshold_y)) mouse[2] = usb->mouse->y;

		 	 HAL_UART_Transmit(&huart2, mouse, 3, 200);
	 }

   if (MouseState==MOUSE_ABSOLUTE)
	 {
		 //MouseButtons = 0;
		 //MouseButtons = MouseButtons|(usb->mouse->buttons[0]<<1);
		 //MouseButtons = MouseButtons||(usb->mouse->buttons[1]);

	   int new_x_coordinate =  (MouseStatus.absolute_mouse_x) + (usb->mouse->x);
	   if (new_x_coordinate < 0)
		   {
		   new_x_coordinate = 0 ;
		   }
	   	   	   else if (new_x_coordinate > MouseStatus.absolute_mouse_max_x)
	   	   	   {
	   	   		   new_x_coordinate = MouseStatus.absolute_mouse_max_x;
	   	   	   }


	   int new_y_coordinate =  (MouseStatus.absolute_mouse_y) + (usb->mouse->y);
	   if (new_y_coordinate < 0)
		   {
		   	   new_y_coordinate = 0;
		   }
	   	   	   else if (new_y_coordinate > MouseStatus.absolute_mouse_max_y )
	   	   	   {
	   	   		   	   new_y_coordinate = MouseStatus.absolute_mouse_max_y;
	   	   	   }




	   MouseStatus.absolute_mouse_x = (uint16_t)new_x_coordinate;
	   MouseStatus.absolute_mouse_y = (uint16_t)new_y_coordinate;

	 }

 }





 //handle even joy
 if(usb!=NULL&&usb->gamepad1!=NULL)
 {
	 //variables
	 uint8_t joy1_package_event[2] = {0};

	 //set data for interrogation mode
	 joydata1 = *usb->gamepad1;

	 //send data for event mode
	if (JoystickState==JOYSTICK_EVENT_REPORTING)
	 {
		if(joydata1_prev!=joydata1)
		{
			joy1_package_event[0] = 0xFE;
			joy1_package_event[1] = MapJoystick(joydata1);
			HAL_UART_Transmit(&huart2, joy1_package_event, 2, 10);
		}
	 }
	joydata1_prev = joydata1;

 }


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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 7800;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart3.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RESET_Pin */
  GPIO_InitStruct.Pin = RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RESET_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */




void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

	printf("UART ERROR: %X\r\n",huart->ErrorCode);
  /* Prevent unused argument(s) compilation warning */
	  if (huart->ErrorCode == HAL_UART_ERROR_ORE){




	        // remove the error condition


	        huart->ErrorCode = HAL_UART_ERROR_NONE;


	        // set the correct state, so that the UART_RX_IT works correctly


	        huart->gState = HAL_UART_STATE_BUSY_RX;


	    }

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_ErrorCallback can be implemented in the user file.
   */
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_SET);

}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  printf("%X \r\n",RxBuffer[0]);
  //Followup requests
  switch (RxPrev)
    {
  	  	  	  case RESET_1:
  	  	  		  	  if (RxBuffer[0]==RESET_2)
  	  	  		  	  {
  	  	  		  	  	 //Mouse state
  	  	  		  	  	 MouseState = MOUSE_RELATIVE;
  	  	  		  	  	 JoystickState = JOYSTICK_EVENT_REPORTING;

  	  	  		  	  	 joydata1 = 0;
  	  	  		  	  	 joydata2 = 0;
  	  	  		  	  	 joydata1_prev = 0;
  	  	  		  	  	 joydata2_prev = 0;

  	  	  		  	     HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
  	  	  		  	  	 return;
  	  	  		  	  }
  	  	  		  	  else
  	  	  		  	  {
  	  	  		  		 RxPrev = 0;
  	  	  		  	  }
  	  	  		  break;

  	    	  case SET_ABSOLUTE_MOUSE_POSITIONING:

	  	  		  MouseStatus.absolute_mouse_max_x = (RxBuffer[0]<<8u)|RxBuffer[1];
	  	  		  MouseStatus.absolute_mouse_max_y = (RxBuffer[2]<<8u)|RxBuffer[3];

  	    	  	  HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
  	    	  	  RxPrev = 0;
  	    	  	  return;
  	    	  break;

  	  	  	  case SET_MOUSE_THRESHOLD:
  	  	  		  	  MouseStatus.mouse_threshold_x = RxBuffer[0];
  	  	  		  	  MouseStatus.mouse_threshold_y = RxBuffer[1];

  	  	  		  	  RxPrev = 0;
  	  	  		  	  HAL_UART_Receive_IT(&huart2, RxBuffer, 1);

  	  	  		  	  return;
  	  	  	  break;

  	  	  	  case SET_MOUSE_SCALE:
  	  				MouseStatus.mouse_scale_x = RxBuffer[0];
  	  				MouseStatus.mouse_scale_y = RxBuffer[1];

				  	HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
				  	RxPrev = 0;
				    return;
			  break;

  	  	  	  case LOAD_MOUSE_POSITION:
  	  	  		  	MouseStatus.absolute_mouse_x = (RxBuffer[1]<<8u)|RxBuffer[2];
  	  	  		  	MouseStatus.absolute_mouse_y = (RxBuffer[3]<<8u)|RxBuffer[4];

  	  	  			HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
  	  	  		  	RxPrev = 0;
  	  	  			return;
  	  	  	  break;



  	  	   	  default:

  	  	  	  break;
    }




  //Initial requests
  switch (RxBuffer[0])
  {
	  case RESET_1:
		  RxPrev =  RESET_1;
	  break;

	  case SET_MOUSE_ACTION :
		  //TODO
	  break;

	  case SET_RELATIVE_MOUSE_POSITIONING :
		  	 MouseState = MOUSE_RELATIVE;
	  break;

	  case SET_ABSOLUTE_MOUSE_POSITIONING :
		    MouseState = MOUSE_ABSOLUTE;
		    RxPrev =  SET_ABSOLUTE_MOUSE_POSITIONING;
		    HAL_UART_Receive_IT(&huart2, RxBuffer, 4);
		    return;
	  break;

	  case SET_MOUSE_KEYCODE :
	  	  		  //TODO
	  break;
	  case SET_MOUSE_SCALE:
	  		  RxPrev = SET_MOUSE_SCALE;
	  		  HAL_UART_Receive_IT(&huart2, RxBuffer, 2);
	  		  return;
	   break;

	  /*0x0D
	  Returns:
	          0xF7       ; absolute mouse position header
	  BUTTONS
	          0000dcba   ; where a is right button down since last interrogation
	                     ; b is right button up since last
	                     ; c is left button down since last
	                     ; d is left button up since last
	          XMSB       ; X coordinate
	          XLSB
	          YMSB       ; Y coordinate
	          YLSB
	   */
	  case INTERROGATE_MOUSE_POSITION:
		  ;
		  uint8_t mouse_absolute_report[6] = {0};

		  mouse_absolute_report[0] = 0xF7;
		  mouse_absolute_report[1] = 0x00; //TODO buttons
		  mouse_absolute_report[2] = *((uint8_t*)&(MouseStatus.absolute_mouse_x)+1);
		  mouse_absolute_report[3] = *((uint8_t*)&(MouseStatus.absolute_mouse_x)+0);
		  mouse_absolute_report[4] = *((uint8_t*)&(MouseStatus.absolute_mouse_y)+1);
		  mouse_absolute_report[5] = *((uint8_t*)&(MouseStatus.absolute_mouse_y)+0);

		  HAL_UART_Transmit(&huart2, mouse_absolute_report, 6, 20);
	  break;


	  case LOAD_MOUSE_POSITION:
		   RxPrev = LOAD_MOUSE_POSITION;
		   HAL_UART_Receive_IT(&huart2, RxBuffer, 5);
		   return;
	  break;


	 //TODO

	  case SET_JOYSTICK_EVENT_REPORTING:
	  	  	JoystickState = JOYSTICK_EVENT_REPORTING;

	  break;

	  case SET_JOYSTICK_INTERROGATION_MODE:
		    JoystickState = JOYSTICK_INTERROGATION_MODE;

	  break;

	  case JOYSTICK_INTERROGATE:
		  ;
		  uint8_t joy_package[3] = {0};
		  joy_package[0]=0xFD;
		  joy_package[1]= MapJoystick(joydata2);
		  joy_package[2]= MapJoystick(joydata1);
		  HAL_UART_Transmit(&huart2, joy_package, 3, 20);
	  break;

	  case SET_MOUSE_THRESHOLD:
		  RxPrev = SET_MOUSE_THRESHOLD;
		  HAL_UART_Receive_IT(&huart2, RxBuffer, 2);
		  return;
	  break;



	  case SET_Y0_AT_BOTTOM:
		  //TODO
	  break;

	  case SET_Y0_AT_TOP:
		  //TODO
	  break;







	  default:
	  break;
  }



  HAL_UART_Receive_IT(&huart2, RxBuffer, 1);

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
