/*
*****************************************************************************
    ���ܣ������ʼ��
    ʱ�䣺2022.02.15
    ��Ա��wjonas
    �޸ļ�¼��
        wjonas 2022.02.15  �޸�
*****************************************************************************
*/ 

#include "config.h"
#include "system.h"

/************************************************************
���ܣ������ʼ��
ʱ�䣺2022.02.15
��Ա��wjonas
����λ�ã�rt_hw_board_init()  ϵͳ����������
��Σ���
���Σ���
************************************************************/
void SysInit(void)
{
    //ϵͳ�δ�ʱ����ʼ��
	SysTickInit();
    
    //LED��ʼ��
	LedGpioInit();
    
    //BEEP��ʼ��
    BeepGpioInit();
    
    //���ڳ�ʼ��
	UsartInit();
    
    //�ⲿ�жϳ�ʼ��
	ExtiInit();
#if	USING_BUTTON    
    //�ж�����Ϊ1ms,���ڰ���ɨ��	
	Timer_Configuration();
#endif
}



