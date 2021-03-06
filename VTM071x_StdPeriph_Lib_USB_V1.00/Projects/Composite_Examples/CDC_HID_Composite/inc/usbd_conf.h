/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  MIC Software Team
  * @version V1.0.0
  * @date    10-09-2018
  * @brief   USB Device configuration file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USBD_CFG_MAX_NUM                1
#define USBD_ITF_MAX_NUM                1
#define USB_MAX_STR_DESC_SIZ            64 
#define USBD_SELF_POWERED               

#define CUSTOMHID_SIZ_REPORT_DESC       39
#define CUSTOMHID_SIZ_CONFIG_DESC       41

#define HID_IN_EP                       0x81
#define HID_OUT_EP                      0x01

/*2 Bytes max*/
#define HID_IN_PACKET                   64
#define HID_OUT_PACKET                  64

#define CDC_IN_EP                       0x83  /* EP1 for data IN */
#define CDC_OUT_EP                      0x03  /* EP3 for data OUT */
#define CDC_CMD_EP                      0x82  /* EP2 for CDC commands */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#define CDC_DATA_MAX_PACKET_SIZE        64   /* Endpoint IN & OUT Packet size */
#define CDC_CMD_PACKET_SZE              64   /* Control Endpoint Packet size */

#define CDC_IN_FRAME_INTERVAL           5    /* Number of frames between IN transfers */
#define APP_RX_DATA_SIZE                2048 /* Total size of IN buffer: 
                                                APP_RX_DATA_SIZE*8/MAX_BAUDARATE*1000 should be > CDC_IN_FRAME_INTERVAL */
#define APP_FOPS                        VCP_fops

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __USBD_CONF__H__ */

/************************ (C) COPYRIGHT MIC *****END OF FILE****/

