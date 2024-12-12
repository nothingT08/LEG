#include "pid.h"
#include "sys.h"
#include "Variable.h"
#include "Remote_Control.h"








float VerticalTurn(float target,float angle,int ID)//陀螺仪数值，转向角度遥控器数值
{
	
	Turn[ID].target=target;
	Turn[ID].err=target-angle;
	
	Turn[ID].actual=Turn[ID].Kd*(Turn[ID].err-Turn[ID].err_last)+Turn[ID].Kp*Turn[ID].err;
	
	Turn[ID].err_last=Turn[ID].err;
	
	return Turn[ID].actual;	
}









