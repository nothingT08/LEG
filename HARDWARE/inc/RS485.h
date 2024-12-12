#ifndef __RS485_H_
#define __RS485_H_
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "Variable.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


extern uint64_t Time_Add;
void TIM5_Init(u32 arr, u32 psc);
extern QueueHandle_t TxCOM1;
#endif

