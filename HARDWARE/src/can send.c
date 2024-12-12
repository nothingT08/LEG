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
	
	TxMessage.StdId=0x501;																																												 					 // 标准标识符 				   																																								 // 设置扩展标示符 
  TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // 标准帧
  TxMessage.RTR=CAN_RTR_Data;		 																																													 // 数据帧
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







//9025控制指令发送函数



/*
	0x91 写入编码器值作为电机零点命令
	0x81 电机
	0x92 读取多圈角度命令 1-7 角度字节
	0x94 读取单圈角度命令 6-7 角度字节6低7高
	0x9c 读取电机的温度、电压、转速、编码器位置 1 23 45 67
	0x80 电机关闭命令 清除运行状态与接收的控制命令
	0x81 电机停止命令 清除电机运行状态
	0x88 电机运行命令
	0xA1 转矩闭环控制命令 45 转矩电流控制高低字节 								电机回复 1温度 23转矩电流 45电机速度 67编码器位置
	0xA2 速度闭环控制命令 4567速度控制 													电机回复 1温度 23转矩电流 45电机速度 67编码器位置
	0xA4 位置控制命令2(多圈角度)    34速度限制字节 4567位置控制字节
	0xA5 位置控制命令3(单圈角度)    1转动方向 45位置控制字节
	0xA6 位置控制命令3(单圈角度)    1转动方向 23速度限制字节 4567位置控制字节


*/


void Can_9025_Send1(int16_t control,int32_t Speed)
{
		CanTxMsg TxMessage;
		
		TxMessage.StdId=0x141;																																												 					 // 标准标识符 				   																																								 // 设置扩展标示符 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // 标准帧
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // 数据帧
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
		
		TxMessage.StdId=0x142;																																												 					 // 标准标识符 				   																																								 // 设置扩展标示符 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // 标准帧
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // 数据帧
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
		
		TxMessage.StdId=0x10|id;																																												 					 // 标准标识符 				   																																								 // 设置扩展标示符 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // 标准帧
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // 数据帧
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
		
		TxMessage.StdId=0x10|id;																																												 					 // 标准标识符 				   																																								 // 设置扩展标示符 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // 标准帧
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // 数据帧
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
		
		TxMessage.StdId=0x10|id;																																												 					 // 标准标识符 				   																																								 // 设置扩展标示符 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // 标准帧
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // 数据帧
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
	
		TxMessage.StdId=0x10|id;																																												 					 // 标准标识符 				   																																								 // 设置扩展标示符 
		TxMessage.IDE=CAN_Id_Standard;						 																																		 					 // 标准帧
		TxMessage.RTR=CAN_RTR_Data;		 																																													 // 数据帧
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







