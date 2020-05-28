/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v1.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_hid.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/




uint8_t HSReady = 0;
uint8_t FSReady = 0;
uint8_t mouse = 0;
static HID_USBDevicesTypeDef usbDev;










/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostHS;
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess1(USBH_HandleTypeDef *phost, uint8_t id);
static void USBH_UserProcess2(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */

//
void mapUSBDevices()
{
//process FS USB
	// check if Device is Ready
	// if it is ready
if (HSReady==1)
{
	HID_HandleTypeDef *HID_Handle1;
	HID_HandleTypeDef *HID_Handle2;
	int currentInterfaceHS = 0;
	currentInterfaceHS = hUsbHostHS.device.current_interface;



		HID_Handle1 = hUsbHostHS.pActiveClass->pData[0];
		USBH_SelectInterface(&hUsbHostHS, 0);

		  switch(HID_Handle1->HID_Desc.RptDesc.type)
		  {
		  	  case REPORT_TYPE_KEYBOARD:
			  {
		  		usbDev.keyboard = USBH_HID_GetKeybdInfo(&hUsbHostHS);
		  		usbDev.keyboardusbhost = &hUsbHostHS;
		  		usbDev.kbd_int=0;
			  }
			  break;

		  	  case REPORT_TYPE_MOUSE:
		  	  {
		  		usbDev.mouse= USBH_HID_GetMouseInfo(&hUsbHostHS);
		  		usbDev.keyboardusbhost = NULL;
		  	  }
		  	 break;

		  	 case REPORT_TYPE_JOYSTICK:
		      {
		    	  usbDev.gamepad1 = USBH_HID_GetGamepadInfo(&hUsbHostHS);
		    	  usbDev.keyboardusbhost = NULL;
		  	   }
		      break;

		  }


	if (hUsbHostHS.pActiveClass->interfaces>1)
	{



		HID_Handle2 = hUsbHostHS.pActiveClass->pData[1];
		USBH_SelectInterface(&hUsbHostHS, 1);

		 switch(HID_Handle2->HID_Desc.RptDesc.type)
		{
			case REPORT_TYPE_KEYBOARD:
			{
				usbDev.keyboard = USBH_HID_GetKeybdInfo(&hUsbHostHS);
				usbDev.keyboardusbhost = &hUsbHostHS;
			    usbDev.kbd_int=1;
			}
			break;

			case REPORT_TYPE_MOUSE:
			{
				usbDev.mouse= USBH_HID_GetMouseInfo(&hUsbHostHS);
				usbDev.keyboardusbhost = NULL;
			}
			break;

			case REPORT_TYPE_JOYSTICK:
			{
				usbDev.gamepad2 = USBH_HID_GetGamepadInfo(&hUsbHostHS);
				usbDev.keyboardusbhost = NULL;
			}
			break;

		}
		 USBH_SelectInterface(&hUsbHostHS, currentInterfaceHS);
	}




}



if (FSReady==1)
{
	HID_HandleTypeDef *HID_Handle1;
	HID_HandleTypeDef *HID_Handle2;

	int currentInterfaceFS = 0;
	currentInterfaceFS = hUsbHostFS.device.current_interface;

		HID_Handle1 = hUsbHostFS.pActiveClass->pData[0];
		USBH_SelectInterface(&hUsbHostFS, 0);

		  switch(HID_Handle1->HID_Desc.RptDesc.type)
		  {
		  	  case REPORT_TYPE_KEYBOARD:
			  {
		  		usbDev.keyboard = USBH_HID_GetKeybdInfo(&hUsbHostFS);
		  		usbDev.keyboardusbhost = &hUsbHostFS;
		  		usbDev.kbd_int=0;
			  }
			  break;

		  	  case REPORT_TYPE_MOUSE:
		  	  {
		  		usbDev.mouse= USBH_HID_GetMouseInfo(&hUsbHostFS);
		  		usbDev.keyboardusbhost = NULL;
		  	  }
		  	 break;

		  	 case REPORT_TYPE_JOYSTICK:
		      {
		    	  usbDev.gamepad2 = USBH_HID_GetGamepadInfo(&hUsbHostFS);
		    	  usbDev.keyboardusbhost = NULL;
		  	   }
		      break;

		  }


	if (hUsbHostFS.pActiveClass->interfaces>1)
	{

		HID_Handle2 = hUsbHostFS.pActiveClass->pData[1];
		USBH_SelectInterface(&hUsbHostFS, 1);

		 switch(HID_Handle2->HID_Desc.RptDesc.type)
		{
			case REPORT_TYPE_KEYBOARD:
			{
				usbDev.keyboard = USBH_HID_GetKeybdInfo(&hUsbHostFS);
				usbDev.keyboardusbhost = &hUsbHostFS;
				usbDev.kbd_int=1;
			}
			break;

			case REPORT_TYPE_MOUSE:
			{
				usbDev.mouse= USBH_HID_GetMouseInfo(&hUsbHostFS);
				usbDev.keyboardusbhost = NULL;
			}
			break;

			case REPORT_TYPE_JOYSTICK:
			{
				usbDev.gamepad1 = USBH_HID_GetGamepadInfo(&hUsbHostFS);
				usbDev.keyboardusbhost = NULL;
			}
			break;

		}
		 USBH_SelectInterface(&hUsbHostFS, currentInterfaceFS);

}


}}

uint8_t *USBH_HID_GetUSBDev()
{
		return &usbDev;
}

/* USER CODE END 1 */

/**
  * Init USB host library, add supported class and start the library
  * @retval None
  */
void MX_USB_HOST_Init(void)
{
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  
  /* USER CODE END USB_HOST_Init_PreTreatment */
  
  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostHS, USBH_UserProcess1, HOST_HS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostHS, USBH_HID_CLASSHS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostHS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PreTreatment */
  
  /* USER CODE END USB_HOST_Init_PreTreatment */
  
  /* Init host Library, add supported class and start the library. */
  if (USBH_Init(&hUsbHostFS, USBH_UserProcess2, HOST_FS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_RegisterClass(&hUsbHostFS, USBH_HID_CLASS) != USBH_OK)
  {
    Error_Handler();
  }
  if (USBH_Start(&hUsbHostFS) != USBH_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_HOST_Init_PostTreatment */
  
  /* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
  /* USB Host Background task */
  USBH_Process(&hUsbHostHS);
  USBH_Process(&hUsbHostFS);
  mapUSBDevices();
}
/*
 * user callback definition
 */
static void USBH_UserProcess1  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_2 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  HSReady = 0;
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  HSReady = 1;
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_2 */
}

static void USBH_UserProcess2  (USBH_HandleTypeDef *phost, uint8_t id)
{
  /* USER CODE BEGIN CALL_BACK_21 */
  switch(id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
  break;

  case HOST_USER_DISCONNECTION:
  Appli_state = APPLICATION_DISCONNECT;
  FSReady = 0;
  break;

  case HOST_USER_CLASS_ACTIVE:
  Appli_state = APPLICATION_READY;
  FSReady = 1;
  break;

  case HOST_USER_CONNECTION:
  Appli_state = APPLICATION_START;
  break;

  default:
  break;
  }
  /* USER CODE END CALL_BACK_21 */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
