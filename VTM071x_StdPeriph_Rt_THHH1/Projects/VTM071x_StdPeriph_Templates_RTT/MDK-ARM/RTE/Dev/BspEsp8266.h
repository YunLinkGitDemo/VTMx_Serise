#ifndef  __BSP_ESP8266_H__
#define	 __BSP_ESP8266_H__







#if defined ( __CC_ARM   )
#pragma anon_unions
#endif



/******************************* ESP8266 数据类型定义 ***************************/
typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;



/******************************* ESP8266 外部全局变量声明 ***************************/
#define RX_BUF_MAX_LEN     512                                     //最大接收缓存字节数

struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {
    __IO uint16_t InfAll;
    struct {
		  __IO uint16_t FramLength       :15;                               // 14:0 
		  __IO uint16_t FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
} ;

extern struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record;

/******************************** ESP8266 连接引脚定义 ***********************************/  
#define      macESP8266_CH_PD_PORT                            GPIO0
#define      macESP8266_CH_PD_PIN                             GPIO_Pin_4

#define      macESP8266_RST_PORT                              GPIO0
#define      macESP8266_RST_PIN                               GPIO_Pin_5

 

#define      macESP8266_USART_BAUD_RATE                       115200

#define      macESP8266_USARTx                                USART1    
#define      macESP8266_USART_TX_PORT                         GPIO0   
#define      macESP8266_USART_TX_PIN                          GPIO_Pin_2
#define      macESP8266_USART_RX_PORT                         GPIO0
#define      macESP8266_USART_RX_PIN                          GPIO_Pin_3
#define      macESP8266_USART_IRQ                             USART1_IRQn
#define      macESP8266_USART_INT_FUN                         USART1_IRQHandler



/*********************************************** ESP8266 函数宏定义 *******************************************/
#define     macESP8266_Usart( fmt, ... )           USART_printf ( macESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
//#define     macPC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
              

#define     macESP8266_CH_ENABLE()                 GPIO_WriteBit(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN, Bit_SET)
#define     macESP8266_CH_DISABLE()                GPIO_WriteBit(macESP8266_CH_PD_PORT, macESP8266_CH_PD_PIN, Bit_RESET)

#define     macESP8266_RST_HIGH_LEVEL()            GPIO_WriteBit ( macESP8266_RST_PORT, macESP8266_RST_PIN, Bit_SET)
#define     macESP8266_RST_LOW_LEVEL()             GPIO_WriteBit ( macESP8266_RST_PORT, macESP8266_RST_PIN, Bit_RESET)



/****************************************** ESP8266 函数声明 ***********************************************/
int                     ESP8266_Init                        ( void );
void                     ESP8266_Rst                         ( void );
bool                     ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, uint32_t waittime );
void                     ESP8266_AT_Test                     ( void );
bool                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
bool                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
bool                     ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
bool                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
bool                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool                     ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t                  ESP8266_Get_LinkStatus              ( void );
uint8_t                  ESP8266_Get_IdLinkStatus            ( void );
uint8_t                  ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
bool                     ESP8266_UnvarnishSend               ( void );
void                     ESP8266_ExitUnvarnishSend           ( void );
bool                     ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId );
char *                   ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );

void ESP8266_InquireIP( void );//查询本机IP地址
void ESP8266_Close_Link ( void );//关闭TCP或UDP连接

#endif


