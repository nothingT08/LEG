#include "RS485.h"
#include "string.h"
#include "Variable.h"
#include "protocol.h"
#include "delay.h"
#include "Remote_Control.h"
#include "arm_math.h"
#include "IMUTASK.h"
QueueHandle_t TxCOM1;






void TIM5_Init(u32 arr, u32 psc)//arr：自动重装值；psc：时钟预分频数
{
	
    NVIC_InitTypeDef  NVIC_InitStruct;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM5时钟使能

    TIM_TimeBaseStructure.TIM_Prescaler = psc; //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = arr; //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //初始化定时器5
		TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM5, ENABLE); 
		NVIC_InitStruct.NVIC_IRQChannel										= TIM5_IRQn;		//中断通道 ，在stm32f4xx.h
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 6;				//抢占优先级
		NVIC_InitStruct.NVIC_IRQChannelSubPriority				= 0;				//响应优先级
		NVIC_InitStruct.NVIC_IRQChannelCmd								= ENABLE;			//中断通道使能 

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


