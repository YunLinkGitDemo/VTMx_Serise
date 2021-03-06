/*
*****************************************************************************
    功能：BEEP的初始化
    时间：2022.02.16
    人员：wjonas
    修改记录：
        wjonas 2022.02.16  修改
*****************************************************************************
*/ 
#include "rtdef.h"

#define Beep_Pin   GPIO_Pin_5
#define Beep_GPIOx GPIO0

/************************************************************
功能：BEEP-GPIO初始化
时间：2022.02.16
人员：wjonas
入参：无
出参：无
************************************************************/
void BeepGpioInit(void);


/************************************************************
功能：Beep-GPIO控制
时间：2022.02.16
人员：wjonas
入参：
      flag
                0  关闭BEEP
                1  开启BEEP    
出参：无
************************************************************/
void BeepControl(rt_uint8_t flag);



