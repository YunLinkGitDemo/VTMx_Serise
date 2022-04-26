/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  MIC Software Team
  * @version V1.0.0
  * @date    10-28-2018
  * @brief   USB Device configuration file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 MIC</center></h2>
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USBD_CFG_MAX_NUM            1
#define USBD_ITF_MAX_NUM            1
#define USB_MAX_STR_DESC_SIZ        64 
#define USBD_SELF_POWERED
#define USB_SUPPORT_USER_STRING_DESC

#define HID_IN_EP                   0x81
#define HID_IN_PACKET               4

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __USBD_CONF__H__ */

/************************ (C) COPYRIGHT MIC *****END OF FILE****/
