
#include "Variable.h"
#include "math.h"

u8 VFlag_state=0;
u8 ZFlag_state=0;
u8 XFlag_state=0;
u8 CFlag_state=0;
u8 RFlag_state=0;
u8 FFlag_state=0;	
u8 CTRL_State =0;
u8 GFlag_state=0;
u8 BFlag_state=0;
u8 MouseZU_State=0;
u8 MouseZB_State=0;

float uint_to_float(int x_int,float x_min,float x_max,int bits)
{
	float span	 = x_max-x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1))+offset;
	
}


int float_to_uint(float x,float x_min,float x_max,int bits)
{
	float span 		= x_max - x_min;
  float offset  = x_min;
	return (int)((x-offset)*((float)((1<<bits)-1))/span);

}


int Mode=0;
int Last_Mode=0;
int RC_Mode;

float YAW_CTRL=0;
float YAW_CTRL_Real=0;
float Distan;
int 	Cutting;

MF9025 MF[2];
float RealSpeed[2];

Super_power_t Super_power;
int PC_Flag;

PID Turn[8];
motor DJ6020[4];
int Cutting;
int SHIFT_PRESS;
KEY_STATE FLAG;

DM8009	 DM[4];
uint16_t chassis_power_buffer;
uint16_t game_progress;

uint8_t RS485_RX_CNT=0;
uint8_t L_Data;
uint8_t BUFF_DMA[64];
uint8_t Flag;
uint8_t RID;
ControlData_t motor_send_data;   //电机控制数据结构体

Send SD[4];
ReceData_t Rec_M[4];
float RAMP_float( float final, float now, float ramp )
{
	  float buffer = 0;
	  buffer = final - now;
		if (buffer > 0)
		{
				if (buffer > ramp)
				{  
						now += ramp;
				}   
				else
				{
						now += buffer;
				}
		}
		else if(buffer <= 0)
		{
				if (buffer < -ramp)
				{
						now += -ramp;
				}
				else
				{
						now += buffer;
				}
		}
		return now;
}




