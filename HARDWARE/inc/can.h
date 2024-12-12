#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    



#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.	



				 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);                   //CAN初始化
u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);              		  //CAN初始化 

void Can_Send_6020_YAW(int16_t balan1,int16_t balan2,int16_t balan3,int16_t balan4);
void Can_Send_6020(int16_t balan1,int16_t balan2,int16_t balan3,int16_t balan4);
void Can_Send_3508_HIGH(int16_t data1,int16_t data2,int16_t data3,int16_t data4);
void Can_Send_3508_LOW(int16_t data1,int16_t data2,int16_t data3,int16_t data4);
void CAN_CMD_SUPERPOWER(int16_t power, int16_t i,float buffer_power);
void Can_9025_Send1(int16_t control,int32_t Speed);
void Can_9025_Send2(int16_t control,int32_t Speed);
void Can_9025_Send3(int16_t control,int32_t Speed);
void Can_9025_Send(int16_t Speed1,int16_t Speed2,int16_t Speed3,int16_t Speed4);

void Can_DM8009_TOR(int16_t id,float Tor);
void Can_DM8009_ZERO(int16_t id,int32_t Speed);
void Can_DM8009_DISABLE(int16_t id,int32_t Speed);
void Can_DM8009_ENABLE(int16_t id,int32_t Speed);
void PlateCan(void);
#endif





