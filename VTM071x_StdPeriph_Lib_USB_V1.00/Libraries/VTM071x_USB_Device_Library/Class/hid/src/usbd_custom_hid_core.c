/**
  ******************************************************************************
  * @file    usbd_hid_core.c
  * @author  MIC Software Team
  * @version V1.0.1
  * @date    10-09-2018
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Custom
  *             - Collection : Application 
  *           
  *      
  *  @endverbatim
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_core.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/ 
uint8_t USBD_HID_Init(void *pdev, uint8_t cfgidx);

uint8_t USBD_HID_DeInit(void  *pdev, uint8_t cfgidx);

uint8_t USBD_HID_Setup(void  *pdev, USB_SETUP_REQ *req);

uint8_t *USBD_HID_GetCfgDesc(uint8_t speed, uint16_t *length);

uint8_t USBD_HID_DataIn(void *pdev, uint8_t epnum);

uint8_t USBD_HID_DataOut(void *pdev, uint8_t epnum);

uint8_t USBD_HID_EP0_RxReady(void *pdev);

uint8_t USBD_HID_Suspend(void);

USBD_Class_cb_TypeDef  USBD_HID_cb = 
{
    USBD_HID_Init,
    USBD_HID_DeInit,
    USBD_HID_Setup,
    NULL, /*EP0_TxSent*/  
    USBD_HID_EP0_RxReady, /*EP0_RxReady*/ /* STATUS STAGE IN */
    USBD_HID_DataIn, /*DataIn*/
    USBD_HID_DataOut, /*DataOut*/
    NULL, /*SOF */    
    USBD_HID_GetCfgDesc, 
    USBD_HID_Suspend,
};

uint8_t Report_buf[128];

uint8_t USBD_HID_Report_ID=0;

static uint32_t  USBD_HID_AltSet = 0;

static uint32_t  USBD_HID_Protocol = 0;

static uint32_t  USBD_HID_IdleState = 0;

static uint32_t  USBD_HID_ReportDescState = 0;

/* USB HID device Configuration Descriptor */
__align(4) const uint8_t USBD_HID_CfgDesc[CUSTOMHID_SIZ_CONFIG_DESC] =
{
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    CUSTOMHID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
    the configuration*/
    0xC0,         /*bmAttributes: bus powered and Support Remote Wake-up */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Custom HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    CUSTOMHID_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Custom HID endpoints ***********/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */
    /*	Endpoint descriptor type */
    HID_IN_EP,     /* bEndpointAddress: Endpoint Address (IN) */
    0x03,          /* bmAttributes: Interrupt endpoint */
    HID_IN_PACKET, /* wMaxPacketSize: 64 Bytes max */
    0x00,
    0x01,          /* bInterval: Polling Interval (1 ms) */
    /* 34 */

    0x07,	         /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
    /*	Endpoint descriptor type */
    HID_OUT_EP,	/* bEndpointAddress: */
    /*	Endpoint Address (OUT) */
    0x03,	/* bmAttributes: Interrupt endpoint */
    HID_OUT_PACKET,	/* wMaxPacketSize: 64 Bytes max  */
    0x00,
    0x04,	/* bInterval: Polling Interval (4 ms) */
    /* 41 */
} ;

__align(4) const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
{
//    0x05, 0x01, //  USAGE_PAGE (Generic Desktop)
//    0x09, 0x00, //  USAGE (0)
//    0xA1, 0x01, //  COLLECTION (Application)
//    0x15, 0x00, //  LOGICAL_MINIMUM (0)
//    0x25, 0xFF, //  LOGICAL_MAXIMUM (255)
//    0x19, 0x00,	//  0x19, 0x01, //  USAGE_MINIMUM (1)
//    0x29, 0xFF,	//  0x29, 0x08, //  USAGE_MAXIMUM (8) 
//    0x95, 0x40, //  REPORT_COUNT (8)
//    0x75, 0x08, //  REPORT_SIZE (8)
//    0x81, 0x02, //  INPUT (Data,Var,Abs)
//    0x19, 0x00,	//  0x19, 0x01, //  USAGE_MINIMUM (1)
//    0x29, 0xff,	//  0x29, 0x08, //  USAGE_MAXIMUM (8) 
//    0x91, 0x02, //  OUTPUT (Data,Var,Abs)
//    0xC0        //  END_COLLECTION
    
    0x05, 0xFF,                    // USAGE_PAGE(User define)
    0x09, 0xFF,                    // USAGE(User define)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x01,                    // USAGE_PAGE(1)
    0x19, 0x00,                    //   USAGE_MINIMUM(0)
    0x29, 0xFF,                    //   USAGE_MAXIMUM(255)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x40,                    //   REPORT_COUNT (64)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x05, 0x02,                    // USAGE_PAGE(2)
    0x19, 0x00,                    //   USAGE_MINIMUM (0)
    0x29, 0xFF,                    //   USAGE_MAXIMUM (255)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (255)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x75, 0x40,                    //   REPORT_SIZE (64)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0xc0                           // END_COLLECTION
}; /* CustomHID_ReportDescriptor */

/* Private function ----------------------------------------------------------*/ 
/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t USBD_HID_Init(void  *pdev, uint8_t cfgidx)
{
    DCD_PMA_Config(pdev, HID_IN_EP, USB_SNG_BUF, HID_IN_TX_ADDRESS);
    DCD_PMA_Config(pdev, HID_OUT_EP, USB_SNG_BUF, HID_OUT_RX_ADDRESS);

    /* Open EP IN */
    DCD_EP_Open(pdev,
                HID_IN_EP,
                HID_IN_PACKET,
                USB_EP_INT);

    /* Open EP OUT */
    DCD_EP_Open(pdev,
                HID_OUT_EP,
                HID_OUT_PACKET,
                USB_EP_INT);

    /*Receive Data*/
    DCD_EP_PrepareRx(pdev, HID_OUT_EP, Report_buf, 64);

    return USBD_OK;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t USBD_HID_DeInit(void  *pdev, uint8_t cfgidx)
{
    /* Close HID EPs */
    DCD_EP_Close(pdev , HID_IN_EP);
    DCD_EP_Close(pdev , HID_OUT_EP);

    return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
uint8_t USBD_HID_Setup (void *pdev, USB_SETUP_REQ *req)
{
    uint8_t USBD_HID_Report_LENGTH=0;
    uint16_t len = 0;
    uint8_t  *pbuf = NULL;

    switch (req->bmRequest & USB_REQ_TYPE_MASK)
    {
        case USB_REQ_TYPE_CLASS :  
            switch (req->bRequest)
            {
                case HID_REQ_SET_PROTOCOL:
                    USBD_HID_Protocol = (uint8_t)(req->wValue);
                break;

                case HID_REQ_GET_PROTOCOL:
                    USBD_CtlSendData(pdev, (uint8_t *)&USBD_HID_Protocol, 1);    
                break;

                case HID_REQ_SET_IDLE:
                    USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
                break;

                case HID_REQ_GET_IDLE:
                    USBD_CtlSendData (pdev, (uint8_t *)&USBD_HID_IdleState, 1);        
                break;

                case HID_REQ_SET_REPORT:
                    USBD_HID_Report_ID = (uint8_t)(req->wValue);
                    USBD_HID_Report_LENGTH = (uint8_t)(req->wLength);
                    USBD_CtlPrepareRx(pdev, Report_buf, USBD_HID_Report_LENGTH);
                break;

                default:
                    USBD_CtlError(pdev, req);
                return USBD_FAIL;
            }
        break;

        case USB_REQ_TYPE_STANDARD:
            switch (req->bRequest)
            {
                case USB_REQ_GET_DESCRIPTOR: 
                    if(req->wValue >> 8 == HID_REPORT_DESC)
                    {
                        len = MIN(CUSTOMHID_SIZ_REPORT_DESC , req->wLength);
                        pbuf = (uint8_t*)CustomHID_ReportDescriptor;
                    }
                    else if(req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
                    {
                        pbuf = (uint8_t*)USBD_HID_CfgDesc + 0x12;
                        len = MIN(USB_HID_DESC_SIZ , req->wLength);
                    }

                    USBD_CtlSendData(pdev, pbuf, len);
                    
                    USBD_HID_ReportDescState = 1;

                break;

                case USB_REQ_GET_INTERFACE :
                    USBD_CtlSendData (pdev, (uint8_t *)&USBD_HID_AltSet, 1);
                break;

                case USB_REQ_SET_INTERFACE :
                    USBD_HID_AltSet = (uint8_t)(req->wValue);
                break;
            }
    }
    
    return USBD_OK;
}

/**
  * @brief  USBD_HID_SendReport 
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport(USB_CORE_HANDLE *pdev, uint8_t *report, uint16_t len)
{
    USB_EP *ep = &pdev->dev.in_ep[HID_IN_EP & 0x7F];

    if (ep->busy == 0)
    {
        /* Check if USB is configured */
        if ((pdev->dev.device_status == USB_CONFIGURED) && (USBD_HID_ReportDescState))
        {
            DCD_EP_Tx(pdev, HID_IN_EP, report, len);
        }
        
        return USBD_OK;
    }
    else
    {
        return USBD_BUSY;
    }
}

/**
  * @brief  USBD_HID_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
uint8_t *USBD_HID_GetCfgDesc(uint8_t speed, uint16_t *length)
{
    *length = sizeof (USBD_HID_CfgDesc);
    
    return (uint8_t*)USBD_HID_CfgDesc;
}

/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t USBD_HID_DataIn(void  *pdev, uint8_t epnum)
{

    
    return USBD_OK;
}

/**
  * @brief  USBD_HID_DataOut
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t USBD_HID_DataOut(void *pdev, uint8_t epnum)
{    
//    DCD_EP_PrepareRx(pdev, HID_IN_EP, Report_buf, 2);

    return USBD_OK;
}

/**
  * @brief  USBD_HID_EP0_RxReady
  *         Handles control request data.
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t USBD_HID_EP0_RxReady(void *pdev)
{    
    return USBD_OK;
}

uint8_t USBD_HID_Suspend(void)
{
    USB_EP *ep = &USB_Device_dev.dev.in_ep[HID_IN_EP & 0x7F];
    
    if (ep->busy == 1)
    {
        ep->busy = 0;
        
        FlushEPnTxFIFO();
        if (ep->doublebuffer)
            FlushEPnTxFIFO();
    }
    
    return USBD_OK;
}

/************************ (C) COPYRIGHT MIC *****END OF FILE****/
