#include "can.h"
#include "pid.h"
#include "delay.h"
#include "Variable.h"
#include "stm32f4xx_can.h" 
#include "judgement_info.h"
#include "Remote_Control.h"

extern 		ext_shoot_data_t						ShootData;
extern 		ext_game_state_t 						GameState;
extern 		ext_power_heat_data_t				PowerHeatData;				//0x0202
extern 		ext_game_robot_state_t			GameRobotStat;				//0x0201


void PlateCan(void)
{
	CanTxMsg TxMessage;
	
	TxMessage.StdId=0x501;																																												 					 // ��׼��ʶ�� 				   																																								 // ������չ��ʾ�� 
  TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // ��׼֡
  TxMessage.RTR=CAN_RTR_Data;		 																																													 // ����֡
  TxMessage.DLC=0x08;			
	
	
	TxMessage.Data[0]=Super_power.volt<<8>>8;
	TxMessage.Data[1]=Super_power.volt>>8;
	TxMessage.Data[2]=GameRobotStat.shooter_id1_17mm_cooling_limit<<8>>8;
	TxMessage.Data[3]=GameRobotStat.shooter_id1_17mm_cooling_limit>>8;
	TxMessage.Data[4]=PowerHeatData.chassis_power_buffer<<8>>8;
	TxMessage.Data[5]=PowerHeatData.chassis_power_buffer>>8;
	TxMessage.Data[6]=PowerHeatData.shooter_id1_17mm_cooling_heat<<8>>8;
	TxMessage.Data[7]=PowerHeatData.shooter_id1_17mm_cooling_heat>>8;
	
	CAN_Transmit(CAN2, &TxMessage);  

}	






TX_Union_data CAN_TX_DATA;

void CAN_CMD_SUPERPOWER(int16_t power, int16_t i,float buffer_power)
{
    CanTxMsg SendCanTxMsg;
		CAN_TX_DATA.TX_Data .power =power ;
    CAN_TX_DATA.TX_Data .i =i;	
		CAN_TX_DATA.TX_Data .buffer_power =buffer_power;
    SendCanTxMsg.StdId = 0x222;
    SendCanTxMsg.IDE = CAN_ID_STD;
    SendCanTxMsg.RTR = CAN_RTR_DATA;
    SendCanTxMsg.DLC = 0x08;
	  for(int i=0;i<sizeof(Send_data);i++)
   { 
		 SendCanTxMsg.Data[i] = CAN_TX_DATA.TX_Data_char [i];
	 }
	
    CAN_Transmit(CAN2, &SendCanTxMsg);
}







//9025����ָ��ͺ���



/*
	0x91 д�������ֵ��Ϊ����������
	0x81 ���
	0x92 ��ȡ��Ȧ�Ƕ����� 1-7 �Ƕ��ֽ�
	0x94 ��ȡ��Ȧ�Ƕ����� 6-7 �Ƕ��ֽ�6��7��
	0x9c ��ȡ������¶ȡ���ѹ��ת�١�������λ�� 1 23 45 67
	0x80 ����ر����� �������״̬����յĿ�������
	0x81 ���ֹͣ���� ����������״̬
	0x88 �����������
	0xA1 ת�رջ��������� 45 ת�ص������Ƹߵ��ֽ� 								����ظ� 1�¶� 23ת�ص��� 45����ٶ� 67������λ��
	0xA2 �ٶȱջ��������� 4567�ٶȿ��� 													����ظ� 1�¶� 23ת�ص��� 45����ٶ� 67������λ��
	0xA4 λ�ÿ�������2(��Ȧ�Ƕ�)    34�ٶ������ֽ� 4567λ�ÿ����ֽ�
	0xA5 λ�ÿ�������3(��Ȧ�Ƕ�)    1ת������ 45λ�ÿ����ֽ�
	0xA6 λ�ÿ�������3(��Ȧ�Ƕ�)    1ת������ 23�ٶ������ֽ� 4567λ�ÿ����ֽ�


*/


void Can_9025_Send1(int16_t control,int32_t Speed)
{
		CanTxMsg TxMessage;
		
		TxMessage.StdId=0x141;																																												 					 // ��׼��ʶ�� 				   																																								 // ������չ��ʾ�� 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // ��׼֡
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // ����֡
		TxMessage.DLC=0x08;		
		TxMessage.Data[0] =	control;
		
		TxMessage.Data[4]	=	Speed<<24>>24;
		TxMessage.Data[5]	=	Speed<<16>>24;
		TxMessage.Data[6]	=	Speed<< 8>>24;
		TxMessage.Data[7]	=	Speed		 >>24;

	
		CAN_Transmit(CAN2, &TxMessage);
		
}



void Can_9025_Send2(int16_t control,int32_t Speed)
{
		CanTxMsg TxMessage;
		
		TxMessage.StdId=0x142;																																												 					 // ��׼��ʶ�� 				   																																								 // ������չ��ʾ�� 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // ��׼֡
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // ����֡
		TxMessage.DLC=0x08;		
		TxMessage.Data[0] =	control;
		
		TxMessage.Data[4]	=	Speed<<24>>24;
		TxMessage.Data[5]	=	Speed<<16>>24;
		TxMessage.Data[6]	=	Speed<< 8>>24;
		TxMessage.Data[7]	=	Speed		 >>24;
	
		CAN_Transmit(CAN2, &TxMessage);
		
}


void Can_DM8009_ENABLE(int16_t id,int32_t Speed)
{
		CanTxMsg TxMessage;
		
		TxMessage.StdId=0x10|id;																																												 					 // ��׼��ʶ�� 				   																																								 // ������չ��ʾ�� 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // ��׼֡
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // ����֡
		TxMessage.DLC=0x08;		
		TxMessage.Data[0] =	0xFF;
		TxMessage.Data[1] =	0xFF;
		TxMessage.Data[2] =	0xFF;
		TxMessage.Data[3] =	0xFF;
		TxMessage.Data[4]	=	0xFF;
		TxMessage.Data[5]	=	0xFF;
		TxMessage.Data[6]	=	0xFF;
		TxMessage.Data[7]	=	0xFC;
	
		CAN_Transmit(CAN1, &TxMessage);
		
}


void Can_DM8009_DISABLE(int16_t id,int32_t Speed)
{
		CanTxMsg TxMessage;
		
		TxMessage.StdId=0x10|id;																																												 					 // ��׼��ʶ�� 				   																																								 // ������չ��ʾ�� 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // ��׼֡
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // ����֡
		TxMessage.DLC=0x08;		
		TxMessage.Data[0] =	0xFF;
		TxMessage.Data[1] =	0xFF;
		TxMessage.Data[2] =	0xFF;
		TxMessage.Data[3] =	0xFF;
		TxMessage.Data[4]	=	0xFF;
		TxMessage.Data[5]	=	0xFF;
		TxMessage.Data[6]	=	0xFF;
		TxMessage.Data[7]	=	0xFD;
	
		CAN_Transmit(CAN1, &TxMessage);   

}

void Can_DM8009_ZERO(int16_t id,int32_t Speed)
{
		CanTxMsg TxMessage;
		
		TxMessage.StdId=0x10|id;																																												 					 // ��׼��ʶ�� 				   																																								 // ������չ��ʾ�� 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // ��׼֡
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // ����֡
		TxMessage.DLC=0x08;		
		TxMessage.Data[0] =	0xFF;
		TxMessage.Data[1] =	0xFF;
		TxMessage.Data[2] =	0xFF;
		TxMessage.Data[3] =	0xFF;
		TxMessage.Data[4]	=	0xFF;
		TxMessage.Data[5]	=	0xFF;
		TxMessage.Data[6]	=	0xFF;
		TxMessage.Data[7]	=	0xFE;
	
		CAN_Transmit(CAN1, &TxMessage);
		
}
int16_t TOR;

void Can_DM8009_TOR(int16_t id,float Tor)
{
		
		CanTxMsg TxMessage;
		TOR=float_to_uint(Tor,T_MIN,T_MAX,12);
	
		TxMessage.StdId=0x10|id;																																												 					 // ��׼��ʶ�� 				   																																								 // ������չ��ʾ�� 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // ��׼֡
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // ����֡
		TxMessage.DLC=0x08;		
		TxMessage.Data[0] =	0;
		TxMessage.Data[1] =	0;
		TxMessage.Data[2] =	0;
		TxMessage.Data[3] =	0;
		TxMessage.Data[4]	=	0;
		TxMessage.Data[5]	=	0;
		TxMessage.Data[6]	=	TOR>>8;
		TxMessage.Data[7]	=	TOR;
	
		CAN_Transmit(CAN1, &TxMessage);
		
}







