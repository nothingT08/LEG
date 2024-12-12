#ifndef _PID_H_
#define _PID_H_
				
float Verticalring(float temp_angle,float gryo,float angle);
float VerticalSpeedring(float aclture,float target,int ID);
float VerticalTurn(float target,float angle,int ID);																			//陀螺仪数值，转向角度遥控器数值

#endif
