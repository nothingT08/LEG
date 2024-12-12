#include "RS485.h"
#include "string.h"
#include "Variable.h"
#include "protocol.h"
#include "delay.h"
#include "Remote_Control.h"
#include "arm_math.h"
#include "IMUTASK.h"
QueueHandle_t TxCOM1;






void TIM5_Init(u32 arr, u32 psc)//arr���Զ���װֵ��psc��ʱ��Ԥ��Ƶ��
{
	
    NVIC_InitTypeDef  NVIC_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM5ʱ��ʹ��

    TIM_TimeBaseStructure.TIM_Prescaler = psc; //��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = arr; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //��ʼ����ʱ��5
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM5, ENABLE); 
		NVIC_InitStruct.NVIC_IRQChannel										= TIM5_IRQn;		//�ж�ͨ�� ����stm32f4xx.h
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 6;				//��ռ���ȼ�
		NVIC_InitStruct.NVIC_IRQChannelSubPriority				= 0;				//��Ӧ���ȼ�
		NVIC_InitStruct.NVIC_IRQChannelCmd								= ENABLE;			//�ж�ͨ��ʹ�� 

    NVIC_Init(&NVIC_InitStruct);
     
}



uint64_t Time_Add=0;
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
	{
		Time_Add++;
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}


