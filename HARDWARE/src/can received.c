#include "can.h"
#include "math.h"
#include "delay.h"
#include "Variable.h"
#include "pid.h"
#include "Remote_Control.h"
#include "judgement_info.h"

CanRxMsg RxMessage;
int time_speed=0;
int speed_zero=1;
extern int16_t Standout[2];
float Real_Speed_Time[2];
float Real_acl[2];
extern 		ext_power_heat_data_t				PowerHeatData;				//0x0202
extern    ext_game_robot_state_t			GameRobotStat;				//0x0201

void CAN1_RX0_IRQHandler(void)
{
	int motor_ID=0;

	CAN_Receive(CAN1, CAN_Filter_FIFO0, &RxMessage);
	

	if((RxMessage.StdId>=0x10)&&(RxMessage.StdId<=0x13))
	{	
		motor_ID=RxMessage.StdId-0x10;	
		DM[motor_ID].ERR					=		RxMessage.Data[0]>>4;
		DM[motor_ID].Encoder			=		uint_to_float(RxMessage.Data[1]<<8|RxMessage.Data[2],P_MIN,P_MAX,16);
		DM[motor_ID].SPEED				=		uint_to_float(RxMessage.Data[3]<<4|RxMessage.Data[4]>>4,V_MIN,V_MAX,12);
		DM[motor_ID].Tor					=		uint_to_float(((RxMessage.Data[4]&0xF)<<8)|RxMessage.Data[5],T_MIN,T_MAX,12);
		DM[motor_ID].T_MOS				=		RxMessage.Data[6];
		DM[motor_ID].T_Rotor			=		RxMessage.Data[7];
	}
		
	if(RxMessage.StdId==0x300)
	{

			Super_power.volt = (uint16_t)(RxMessage.Data[1] << 8 | RxMessage.Data[0]);
			Super_power.power = (uint16_t)(RxMessage.Data[3] << 8 | RxMessage.Data[2]);
			Super_power.current = (uint16_t)(RxMessage.Data[5] << 8 | RxMessage.Data[4]);

	}	
	
	
	
	
	
	
}
	
	
typedef union 
{
	unsigned char data[4];
	float         Power;

}POWER;



POWER PPow;
float Speed_m;
float Stop_Time=0;
extern int Dodge_Speed;
void CAN2_RX1_IRQHandler(void)
{
	int motor_ID=0;
	CanRxMsg RxMessage;
	CAN_Receive(CAN2, CAN_Filter_FIFO1, &RxMessage);
	
	if((RxMessage.StdId>=0x10)&&(RxMessage.StdId<=0x13))
	{	
		motor_ID=RxMessage.StdId-0x10;	
		DM[motor_ID].ERR					=		RxMessage.Data[0]>>4;
		DM[motor_ID].Encoder			=		uint_to_float(RxMessage.Data[1]<<8|RxMessage.Data[2],P_MIN,P_MAX,16);
		DM[motor_ID].SPEED				=		uint_to_float(RxMessage.Data[3]<<4|RxMessage.Data[4]>>4,V_MIN,V_MAX,12);
		DM[motor_ID].Tor					=		uint_to_float(((RxMessage.Data[4]&0xF)<<8)|RxMessage.Data[5],T_MIN,T_MAX,12);
		DM[motor_ID].T_MOS				=		RxMessage.Data[6];
		DM[motor_ID].T_Rotor			=		RxMessage.Data[7];
	}
	
	
	
	
	if(RxMessage.StdId>=0x205&&RxMessage.StdId<=0x208)	
	{
			motor_ID=RxMessage.StdId-0x205;																																						///////////////////////////电调ID接收
			
				
			if((DJ6020[motor_ID].Angle-DJ6020[motor_ID].angleLast)>4096)                                                 \
				{ 
					
					DJ6020[motor_ID].CircleNUM--;
					
				}                                                                    																			 \
			else if((DJ6020[motor_ID].Angle-DJ6020[motor_ID].angleLast)<-4096)                                           \
				{
					
					DJ6020[motor_ID].CircleNUM++;
					
				}  
				
				
				
			DJ6020[motor_ID].angle[0]					=			RxMessage.Data[0];
			DJ6020[motor_ID].angle[1]					=			RxMessage.Data[1];
			DJ6020[motor_ID].rpm[0]						=			RxMessage.Data[2];
			DJ6020[motor_ID].rpm[1]						=			RxMessage.Data[3];
			DJ6020[motor_ID].torque[0]				=			RxMessage.Data[4];
			DJ6020[motor_ID].torque[1]				=			RxMessage.Data[5];
			DJ6020[motor_ID].null[0]					=			RxMessage.Data[6];
			DJ6020[motor_ID].null[1]					=			RxMessage.Data[7];
				
			DJ6020[motor_ID].angleLast  		  =     DJ6020[motor_ID].Angle;
			DJ6020[motor_ID].Angle						=			DJ6020[motor_ID].angle[0]*256+DJ6020[motor_ID].angle[1];	
			DJ6020[motor_ID].Realrpm					=			DJ6020[motor_ID].rpm[0]*256+DJ6020[motor_ID].rpm[1];
			DJ6020[motor_ID].RealAngle				=			(DJ6020[motor_ID].Angle+DJ6020[motor_ID].CircleNUM*8192)/8192.0f*360.0f;
			
	}
	
	
	switch(RxMessage.StdId)
	{	
		case 0x400:
		{
			Last_Mode=RC_Mode;
			RC_Mode=(uint16_t)RxMessage.Data[0];
			SHIFT_PRESS=RxMessage.Data[6];
			Cutting					=		RxMessage.Data[7];

			if(Last_Mode==0 && RC_Mode==1)
			{
				RC_Mode=1;
			}
			if(Last_Mode==1 && RC_Mode==2)
			{
				RC_Mode=2;
			}	
			PPow.data[0]=RxMessage.Data[1];
			PPow.data[1]=RxMessage.Data[2];
			PPow.data[2]=RxMessage.Data[3];
			PPow.data[3]=RxMessage.Data[4];
			
			PowerHeatData.chassis_power=PPow.Power;
		}
		break;	
		
		
		case 0x401:
		{	
		
			//RC_Mode						=	 	RxMessage.Data[0]|RxMessage.Data[1]<<8;
			FLAG.RX[0]				=		RxMessage.Data[0];
			FLAG.RX[1]				=		RxMessage.Data[1];	
			rc_ctrl.rc.ch[2]	=		RxMessage.Data[2]|RxMessage.Data[3]<<8;
			rc_ctrl.rc.ch[3]	=		RxMessage.Data[4]|RxMessage.Data[5]<<8;
			rc_ctrl.rc.ch[4]	=		RxMessage.Data[6]|RxMessage.Data[7]<<8;
			

		}
		break;
	case 0x402:
		{	
			rc_ctrl.rc.ch[0]	=	 	RxMessage.Data[0]|RxMessage.Data[1]<<8;
			rc_ctrl.rc.ch[1]	=		RxMessage.Data[2]|RxMessage.Data[3]<<8;
			PowerHeatData.chassis_power_buffer=RxMessage.Data[4]|RxMessage.Data[5]<<8;
			GameRobotStat.chassis_power_limit=RxMessage.Data[6]|RxMessage.Data[7]<<8;
			
		}
		break;
	}
	
	
	
	
	
	

	if(RxMessage.StdId==0x141||RxMessage.StdId==0x142)
	{
		
		motor_ID=RxMessage.StdId-0x140;																																						///////////////////////////电调ID接收
		motor_ID=motor_ID-1;
		Real_Speed_Time[motor_ID]				=	RealSpeed[motor_ID];
		MF[motor_ID].Motor_Temp					= RxMessage.Data[1];
		MF[motor_ID].Iq[0] 							= RxMessage.Data[2];
		MF[motor_ID].Iq[1] 							= RxMessage.Data[3];
		MF[motor_ID].SPEED[0] 					= RxMessage.Data[4];
		MF[motor_ID].SPEED[1] 					= RxMessage.Data[5];
		MF[motor_ID].Encoder[0] 				= RxMessage.Data[6];
		MF[motor_ID].Encoder[1]					= RxMessage.Data[7];
		MF[motor_ID].RealEncoder				=	(int16_t)MF[motor_ID].Encoder[0]|(int16_t)(MF[motor_ID].Encoder[1])<<8;
		MF[motor_ID].RealSpeed					=	(int16_t)MF[motor_ID].SPEED[0]|(int16_t)MF[motor_ID].SPEED[1]<<8;
		MF[motor_ID].RealIq							=	(int16_t)MF[motor_ID].Iq[0]|(int16_t)MF[motor_ID].Iq[1]<<8;
		
		RealSpeed[motor_ID]							=	-MF[motor_ID].RealSpeed*TheattoRad*(-0.078f);
		Real_acl[motor_ID]							=RealSpeed[motor_ID]-Real_Speed_Time[motor_ID];
		//if(fabs(RealSpeed[motor_ID]-Real_Speed_Time[motor_ID])>10 || fabs(RealSpeed[motor_ID])>30)
		if(Speed_m<Real_acl[motor_ID])
		{
		
			Speed_m=Real_acl[motor_ID];
		
		}
		
		Limit_Min_Max(RealSpeed[motor_ID],-4.5f,4.5f);
		if(fabs(RealSpeed[motor_ID])>3.4f)
		{
			time_speed++;
			Stop_Time=1;
			
			
		}
		if(time_speed!=0||Stop_Time!=0)
		{
			
			speed_zero=0;
			time_speed=0;
			RealSpeed[motor_ID]=Real_Speed_Time[motor_ID];
			Stop_Time--;
		}
		else
		{
			speed_zero=1;
		}
		

		if((MF[motor_ID].RealEncoder-MF[motor_ID].EncoderLast)>32767)
				MF[motor_ID].CircleNUM--;
		else if((MF[motor_ID].RealEncoder-MF[motor_ID].EncoderLast)<-32767)
				MF[motor_ID].CircleNUM++;
		
		MF[motor_ID].EncoderLast=MF[motor_ID].RealEncoder;

	}
	

	
	if(RxMessage.StdId==0x300)
	{

			Super_power.volt = (uint16_t)(RxMessage.Data[1] << 8 | RxMessage.Data[0]);
			Super_power.power = (uint16_t)(RxMessage.Data[3] << 8 | RxMessage.Data[2]);
			Super_power.current = (uint16_t)(RxMessage.Data[5] << 8 | RxMessage.Data[4]);

	}
	
	
	
	
}
	
