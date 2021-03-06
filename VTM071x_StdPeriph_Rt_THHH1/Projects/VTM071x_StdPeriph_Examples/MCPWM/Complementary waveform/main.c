/**
  ******************************************************************************
  * @file    Project/VTM071x_StdPeriph_Templates/main.c 
  * @author  MIC Software Team 
  * @version V1.0.0
  * @date    03/01/2018
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MIC SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 MIC</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "VTM071x.h"

/** @addtogroup VTM071x_StdPeriph_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USART0_CLOCK_DIV	1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void USART_Configuration(void);
void MCPWM_Configuration(void);
void MCPWM_Stop(uint32_t MCPWM_Channel_COM);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/
/* MCPWM??????????????????????????????????????????????????????Limit??????????????????????????????????????????????????? */
void MCPWM_Stop(uint32_t MCPWM_Channel_COM)
{
    /* ???????????????????????? */
    if (MCPWM_Channel_COM & MCPWM_CHANNEL_0)													
    {
        MCPWM->INTF_CLR = MCPWM_IT_ILIM0;
        while ((MCPWM->INTF & MCPWM_IT_ILIM0) == 0);
    }
    else if (MCPWM_Channel_COM & MCPWM_CHANNEL_1)
    {
        MCPWM->INTF_CLR = MCPWM_IT_ILIM1;
        while ((MCPWM->INTF & MCPWM_IT_ILIM1) == 0);
    }
    else if (MCPWM_Channel_COM & MCPWM_CHANNEL_2)
    {
        MCPWM->INTF_CLR = MCPWM_IT_ILIM2;
        while ((MCPWM->INTF & MCPWM_IT_ILIM2) == 0);
    }
    
    /* ??????MCPWM */
    MCPWM_Cmd(MCPWM_Channel_COM, DISABLE);
}

void Delay_us(uint32_t us)
{
    SysTick->LOAD = us * 96;
    SysTick->VAL  = (0x00);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;

    /* Waiting for down-count to zero */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
}

void Delay_ms(uint32_t ms)
{
    int i;
    
    for (i=0; i<ms; i++)
    {
        Delay_us(1000);
    }
}
    
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main()
{
    /* USART */
    USART_Configuration();
    printf("\r\n-----------------|- VTM071x MCPWM -|-----------------\r\n");    
    
    MCPWM_Configuration();
    
	while(1)
	{
        /* ??????100ms */
        Delay_ms(100);
        /* ??????MCPWM????????????????????????????????????????????????????????? */
        MCPWM_Stop(MCPWM_CHANNEL_0 | MCPWM_CHANNEL_1 | MCPWM_CHANNEL_2);
        /* ??????100ms */
        Delay_ms(100);
        /* ????????????MCPWM */
        MCPWM_Cmd(MCPWM_CHANNEL_0 | MCPWM_CHANNEL_1 | MCPWM_CHANNEL_2, ENABLE);
	}
}

/**
  * @brief  Configuare MCPWM.
  * @param  None
  * @retval None
  */
void MCPWM_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    MCPWM_InitTypeDef MCPWM_InitStructure;

    // MC0A0--GPIO2-6
    // MC0A1--GPIO2-0
    // MC0A2--GPIO2-1
    // MC0B0--GPIO3-1
    // MC0B1--GPIO3-2
    // MC0B2--GPIO3-3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;																		//GPIO2_6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;													//?????????50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_2;															//??????????????????2
    GPIO_Init(GPIO2, &GPIO_InitStructure);																			//GPIO2

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;																		//GPIO2_0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_1;															//??????????????????1
    GPIO_Init(GPIO2, &GPIO_InitStructure);																			//GPIO2

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;																		//GPIO2_1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_1;															//??????????????????1
    GPIO_Init(GPIO2, &GPIO_InitStructure);																			//GPIO2
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;																		//GPIO3_1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_3;															//??????????????????3
    GPIO_Init(GPIO3, &GPIO_InitStructure);																			//GPIO3

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;																		//GPIO3_2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_3;															//??????????????????3
    GPIO_Init(GPIO3, &GPIO_InitStructure);																			//GPIO3

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;																		//GPIO3_3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_3;															//??????????????????3
    GPIO_Init(GPIO3, &GPIO_InitStructure);																			//GPIO3_3
												//
    /* MCPWM Clock */
    RCC_APBPeriphClockCmd(RCC_AHBCLKCTRL_MCPWM, ENABLE);												//??????MCPWM??????
    MCPWM_DeInit();																															//?????????MCPWM

    /* Channel0 100KHz, Duty 60% */
    MCPWM_InitStructure.MCPWM_Channel = MCPWM_CHANNEL_0;												//??????0
    MCPWM_InitStructure.MCPWM_Match = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 * 2 - 1; //2us
    MCPWM_InitStructure.MCPWM_Limit = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 * 5 - 1; //5us
    MCPWM_InitStructure.MCPWM_DeadTime = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 / 2 - 1;  //0.5us
    MCPWM_InitStructure.MCPWM_Center = MCPWM_CENTER_CA;													//?????????????????????
    MCPWM_InitStructure.MCPWM_Update = MCPWM_Update_EN;													//?????????????????????
    MCPWM_InitStructure.MCPWM_Polarity = MCPWM_Polarity_ILAH;										//MCPWM??????
    MCPWM_Init(&MCPWM_InitStructure);																						//MCPWM

    /* Channel1 100KHz, Duty 40% */
    MCPWM_InitStructure.MCPWM_Channel = MCPWM_CHANNEL_1;												//??????1
    MCPWM_InitStructure.MCPWM_Match = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 * 3 - 1; //3us
    MCPWM_InitStructure.MCPWM_Limit = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 * 5 - 1; //5us
    MCPWM_InitStructure.MCPWM_DeadTime = 0;
    MCPWM_InitStructure.MCPWM_Center = MCPWM_CENTER_CA;													//?????????????????????
    MCPWM_InitStructure.MCPWM_Update = MCPWM_Update_EN;													//?????????????????????
    MCPWM_InitStructure.MCPWM_Polarity = MCPWM_Polarity_ILAH;										//MCPWM??????
    MCPWM_Init(&MCPWM_InitStructure);																						//MCPWM

    /* Channel2 200KHz, Duty 60% */
    MCPWM_InitStructure.MCPWM_Channel = MCPWM_CHANNEL_2;												//??????2
    MCPWM_InitStructure.MCPWM_Match = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 * 2 - 1; //2us
    MCPWM_InitStructure.MCPWM_Limit = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 * 5 - 1; //5us
    MCPWM_InitStructure.MCPWM_DeadTime = RCC_GetClocksFreq(RCC_CLOCKFREQ_SYSAHBCLK) / 1000000 / 2 - 1;  //0.5us
    MCPWM_InitStructure.MCPWM_Center = MCPWM_CENTER_EA;													//?????????????????????
    MCPWM_InitStructure.MCPWM_Update = MCPWM_Update_EN;													//?????????????????????
    MCPWM_InitStructure.MCPWM_Polarity = MCPWM_Polarity_ILAH;										//MCPWM??????
    MCPWM_Init(&MCPWM_InitStructure);																						//MCPWM

    MCPWM_Cmd(MCPWM_CHANNEL_0 | MCPWM_CHANNEL_1 | MCPWM_CHANNEL_2, ENABLE);			//??????MCPWM??????0???MCPWM??????1???MCPWM??????2
}

/**
  * @brief  Configuare USART.
  * @param  None
  * @retval None
  */
void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;																			//?????????
    GPIO_InitTypeDef GPIO_InitStructure;																				//?????????

    RCC_APBPeriphClockCmd(RCC_AHBCLKCTRL_USART0,ENABLE);												//??????USART0??????
    RCC_USARTCLKSel(RCC_USARTCLK_SOURCE_SYSPLL);																//???????????????
    RCC_SETCLKDivider(RCC_CLOCKFREQ_USART0CLK, USART0_CLOCK_DIV);								//????????????????????????

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;																		//GPIO1_1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;													//?????????50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_1;															//??????????????????1
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;														//????????????
    GPIO_Init(GPIO1, &GPIO_InitStructure);																			//GPIO1

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;																		//GPIO1_2
    GPIO_Init(GPIO1, &GPIO_InitStructure);																			//GPIO1

    USART_DeInit(USART0);																												//???USART0???????????????????????????
    USART_StructInit(&USART_InitStructure);																			//???USART_InitStructure???????????????????????????????????????
    USART_Init(USART0, &USART_InitStructure);																		//????????????????????????????????????USART0?????????????????????
    USART_Cmd(USART0, ENABLE);																									//??????USART0??????
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    USART_SendData(USART0, (uint8_t) ch);

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART0, USART_FLAG_TXFE) == RESET)
    {}

    return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    printf("Wrong parameters value: file %s on line %d\r\n", file, line);

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

