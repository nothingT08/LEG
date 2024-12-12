#ifndef _TaskInit_H_
#define _TaskInit_H_
#include "sys.h"
#include "can.h"
#include "pid.h"
//#include "math.h"
#include "delay.h"
#include "Limit.h"
#include "LASER.h"
#include "RS485.h"
#include "kalaman.h"
#include "IMUTASK.h"
#include "Variable.h"
#include "FreeRTOS.h"
#include "stm32f4xx.h"  
#include "usart_dr16.h"
#include "task.h"
#include "Judge_Task.h"
#include "RM_Client_UI.h"
#include "judgement_info.h"
#include "Remote_Control.h"



void Task_Init(void);
void SYSTEM_Init(void);
void IMU_task(void  *pvParameters);
void SEND_task(void *pvParameters);
void Solve_task(void *pvParameters);
void start_task(void *pvParameters);
void SHOOT_task(void *pvParameters);
void GIMBAL_task(void *pvParameters);
void CHASSIS_task(void *pvParameters);
void PC_Receive_task(void *pvParameters);




#endif
