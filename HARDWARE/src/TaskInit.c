
#define ARM_MATH_CM4 1
#include "TaskInit.h"
#include "arm_math.h"
	
float walkkp=193.9393f;																						//????????????????????????????????????????????????????
float kd_Turn[4]={-1.55f,-1.55f,0,0};															//??????????KP????????????????????
float FN[2],FP[2];
unsigned char Tim_Flag=1;																					//????????????¦Ë???????????????????????
unsigned char LEG_Flag=0;																					//?????????¦Ë????????????????????????
float Wrad=0;																											//??????????err
uint64_t Time_Last;																								//??????????????????????????????§Ö?????????????????????????????????
uint64_t Time_Delta;
uint64_t Time_RUN;
uint8_t Zero=1;																										//????????????????¦Ë???§Ø??????
float Pitch_Last[2];

int16_t Standout[2]={0,0};																				//???????????????
int16_t Standout_Last[2]={0,0};
int Ctrl_state=0;
int Invincible_State;
extern int speed_zero;
extern ext_game_state_t 				GameState;
extern ext_power_heat_data_t		PowerHeatData;				//0x0202
extern ext_game_robot_state_t		GameRobotStat;				//0x0201

TaskHandle_t 	CHASSISTask_Handler;
TaskHandle_t 	LEGTask_Handler;
TaskHandle_t  JUDGE_TASK_Handler;
TaskHandle_t 	UI_TASK_Handler;
TaskHandle_t 	StartTask_Handler;
TaskHandle_t 	IMUTask_Handler;
TaskHandle_t 	SolveTask_Handler;

void LEG_task(void *pvParameters);
static void Super_power_ctrl(void);
void chassis_mode_set(void);

static u8 EFlag_state = 0;

void Task_Init(void)
{
																												//???????????
    xTaskCreate((TaskFunction_t )start_task,            //??????
                (const char*    )"start_task",          //????????
                (uint16_t       )START_STK_SIZE,        //????????§³
                (void*          )NULL,                  //????????????????
                (UBaseType_t    )START_TASK_PRIO,       //?????????
                (TaskHandle_t*  )&StartTask_Handler);   //??????              
    vTaskStartScheduler();          										//???????????
}



																													
void start_task(void *pvParameters)											//?????????
{
  taskENTER_CRITICAL();   															//?????????
//	xTaskCreate		((TaskFunction_t )Judge_task, 
//								(const char*    )"Judge_task", 
//								(uint16_t       )JUDGE_TASK_SIZE, 
//								(void*          )NULL, 
//								(UBaseType_t    )JUDGE_TASK_PRIO,  
//								(TaskHandle_t*  )&JUDGE_TASK_Handler);	
//	
//	xTaskCreate		((TaskFunction_t )UI_task, 			
//								(const char* 		 )"UI_task", 			
//								(uint16_t 			 )UI_TASK_SIZE, 			
//								(void*					 )NULL, 							
//								(UBaseType_t		 )UI_TASK_PRIO, 			
//								(TaskHandle_t* 	 )&UI_TASK_Handler); 	
																												//????????????
  xTaskCreate		((TaskFunction_t )CHASSIS_task,     
                (const char*    )"CHASSIS_task",   
                (uint16_t       )CHASSIS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )CHASSIS_TASK_PRIO,
                (TaskHandle_t*  )&CHASSISTask_Handler); 
								
//								
	xTaskCreate		((TaskFunction_t )IMU_task,
								(const char*    )"IMU_task",
								(uint16_t       )IMU_STK_SIZE,
								(void*          )NULL,
								(UBaseType_t    )IMU_TASK_PRIO,
								(TaskHandle_t*  )&IMUTask_Handler);

								
	xTaskCreate		((TaskFunction_t )Solve_task,             
                (const char*    )"Solve_task",           
                (uint16_t       )Solve_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Solve_TASK_PRIO,        
                (TaskHandle_t*  )&SolveTask_Handler);   
								

  vTaskDelete(StartTask_Handler); 											//??????????
  taskEXIT_CRITICAL();            											//????????
}




void BSP_Init(void)
{
    //MPU?????
    while(MPU6500_Init());
    IMU_Calibration();
    accel_mat_init();
    ACCEL_Calibration();
    MPUHEAT_configuration();
}



void SYSTEM_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);																		//???????§Ø??????????4		//????????????
	BSP_Init();
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_Normal);						//CAN???????????,??????1Mhz
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_Normal);						//CAN???????????,??????1Mhz			

	
	remote_control_init();
	delay_init(168);
	TIM5_Init(400-1,21-1);																														//??????????????????ÂC?????????????
	Kalman_Filter_Init(&Kalman0,2,0,2);																								//?????????????????????????????????????????
	Kalman_Filter_Init(&Kalman2,2,0,2);

	
	
	Task_Init();	
}
float LEG0=0.145f,LEG1=0.295f;
float Goal_x[2]={0.0775f,0.0775f},Goal_y[2]={0.22f,0.22f},Interval=0.155f;		//LEG0?? LEG1??
float real_x[2],real_y[2];		//xita1?0???

float real_x[2],real_y[2];



float u[4];
float SpeedOut[2]={0,0};
//??????????
int PIANCHA=0;
int Dodge_Speed=1;
int BEhind=0;
void CHASSIS_task(void *pvParameters)
{
	Turn[0].Kd=900;//-250.0f;
	Turn[1].Kd=900;//-250.0f;
	Turn[0].Kp=40;//-1.00f;
	Turn[1].Kp=40;//-1.00f;
	while(1)							
	{

		PIANCHA=DJ6020[0].Angle-YawMedian;
		if(PIANCHA>4096)
			PIANCHA=PIANCHA-8192;
		else if(PIANCHA<-4096)
			PIANCHA=PIANCHA+8192;
		if(RC_Mode==CHASSIS_RELAX)
		{
			Can_9025_Send1(0x80,0);
			vTaskDelay(1);
			Can_9025_Send2(0x80,0);
		}
		else if(RC_Mode==CHASSIS_INIT)
		{
				Wrad=0;
				YAW_CTRL=DJ6020[0].RealAngle;
				Can_9025_Send1(0x88,0);
				vTaskDelay(1);
				Can_9025_Send2(0x88,0);
				
		
		}
		else if(RC_Mode==CHASSIS_FOLLOW_GIMBAL||RC_Mode==CHASSIS_DODGE_MODE)
		{
			if(RC_Mode==CHASSIS_FOLLOW_GIMBAL)
			{
				if(FLAG.Key_State.B)
				{				
					YAW_CTRL_Real=YAW_CTRL+90.0f;
				}
				else
				{
					YAW_CTRL_Real=YAW_CTRL;
				}
				if(FLAG.Key_State.Z)
				{
					YAW_CTRL_Real=YAW_CTRL+180.0f;
				}
				if(FLAG.Key_State.F)
				{
					SpeedOut[0]=VerticalTurn(YAW_CTRL_Real+90.0f,YAW_CTRL_Real,0);
					SpeedOut[1]=VerticalTurn(YAW_CTRL_Real+90.0f,YAW_CTRL_Real,1);	
					Dodge_Speed=0;
				}
				else
				{
					Dodge_Speed=1;///??????§³??????¦Ë??????§³??????????????????
					SpeedOut[0]=VerticalTurn(YAW_CTRL_Real,DJ6020[0].RealAngle,0);
					SpeedOut[1]=VerticalTurn(YAW_CTRL_Real,DJ6020[0].RealAngle,1);	
				
				}
						////????wrad???//err???????????			?????????????????  ??????????????
			}
			else if(RC_Mode==CHASSIS_DODGE_MODE)
			{
					SpeedOut[0]=VerticalTurn(YAW_CTRL_Real+10.0f,YAW_CTRL_Real,0);
					SpeedOut[1]=VerticalTurn(YAW_CTRL_Real+10.0f,YAW_CTRL_Real,1);	
			
			
			}

			
			
			//????????????????§µ??????§¹??§³???????
			Standout[0]=(0.1f*(u[0]*walkkp+SpeedOut[0])+Standout_Last[0]*0.9f)*Zero*speed_zero;
			Standout[1]=(0.1f*(u[2]*walkkp-SpeedOut[1])+Standout_Last[1]*0.9f)*Zero*speed_zero;

			
			//???????????????????????????
			if((FN[0]<195&&FN[1]<195)&&!Tim_Flag&&LEG_Flag)
			{
				Standout[0]=0;
				Standout[1]=0;
			}
			
			
			
			
//			if(PowerHeatData.chassis_power_buffer <20)
//			{
//				if(Super_power.volt>15000.0f)
//				{
//					Shift_Flag =1;
//					Limit_Min_Max(Standout[0],-469*4.0f,469*4.0f);
//					Limit_Min_Max(Standout[1],-469*4.0f,469*4.0f);
//				}
//				else if(Super_power.volt<15000.0f)
//				{	
//					Shift_Flag =2;
//					if(GameRobotStat.chassis_power_limit==60)
//					{
//						Limit_Min_Max(Standout[0],-850,850);
//						Limit_Min_Max(Standout[1],-850,850);
//					
//					}
//					else if(GameRobotStat.chassis_power_limit==80)
//					{
//						Limit_Min_Max(Standout[0],-900,900);
//						Limit_Min_Max(Standout[1],-900,900);
//					
//					}
//					else if(GameRobotStat.chassis_power_limit==100)
//					{
//						Limit_Min_Max(Standout[0],-1100,1100);
//						Limit_Min_Max(Standout[1],-1100,1100);
//					
//					}
//					else
//					{
//						Limit_Min_Max(Standout[0],-800,800);
//						Limit_Min_Max(Standout[1],-800,800);
//					}
//				}
//			}
//			else
//			{
//				Shift_Flag =2;
//				Limit_Min_Max(Standout[0],-469*4.0f,469*4.0f);
//				Limit_Min_Max(Standout[1],-469*4.0f,469*4.0f);
//			}
			Limit_Min_Max(Standout[0],-469*4.0f,469*4.0f);
			Limit_Min_Max(Standout[1],-469*4.0f,469*4.0f);
			Standout_Last[0]=Standout[0];
			Standout_Last[1]=Standout[1];
			Can_9025_Send1(0xA1,(Standout[0])*Zero*speed_zero);//Standout[0]*Zero);							//??§Õ?????????????//????0???????????
			vTaskDelay(1);  
			Can_9025_Send2(0xA1,(-Standout[1])*Zero*speed_zero);//-Standout[1]*Zero);							//??§Õ?????????????//????0???????????
		}
    vTaskDelay(1);                          															 				
	}
}


float e[2], f[2], c[2], g[2],T1[2],T2[2],F[2],Tp[2],Fx[2],Fy[2],Fy_Last[2];
float d[2], x1[2], x2[2], y1[2], y2[2], x3[2], y3[2], x4_1[2], y4_1[2], x4_2[2], y4_2[2], a[2], h[2], xita1[2], xita2[2], L0[2],  xita3[2], xita4[2];
float Err_Last_or[4]={0,0},Err_or[4]={0,0},Kp_Leg[8]={50,0.00f,50,0.00f,1.1f,1.1f,0,0},Kd_Leg[8]={200,-20,200,-20},Intenger[4]={0,0,0,0},Ki_Leg[4]={0,0,0,0};

///////////////////////////////////////////////////////////******************************************************************?*?////////////////////////////////////////////////////////////////////////
float gama[2];
float TTOR[4]={0,0,0,0};

////////////////////////////////////////////////////////////////////////****************************??????????***********************************/////////////////////////////////////////////////////////////////////////////////////





float F_front[2]={70.5f,70.5f};
float xita[2];
float J[8];
float xita_Last[2];

float FT[2],FTTP[2];

//////////////////////////////////////////////////////////////////*********************************LQR????*****************************************///////////////////////////////////////////////////////////////

float Ks[2][12];
float Ka[12][4]=
{
	
	
	
	
	//????????Matlab??????????excel?????????????
//	
{-260.6291f,	358.3041f,	-225.0927f,	-12.3301f},
{-66.8655f,	78.9871f,	-49.9421f,	-7.789f},
{-0.0014f,	0.0026f,	-0.0016f,	-0.0097f},
{-1.5754f,	3.0921f,	-1.8395f,	-11.4914f},
{-326.4396f,	503.0515f,	-313.4878f,	98.79f},
{-72.0037f,	111.7017f,	-68.4379f,	22.5202f},
{344.5236f,	-359.1774f,	106.0982f,	24.1084f},
{-78.2247f,	128.3713f,	-87.6769f,	26.6172f},
{0.0231f,	-0.0094f,	-0.0098f,	0.0046f},
{27.3173f,	-11.0504f,	-11.6758f,	5.5121f},
{798.5958f,	-1031.2738f,	489.7048f,	81.1199f},
{192.8528f,	-245.1911f,	112.9921f,	21.2932f}

//{-124.3026f,	192.12f,	-132.4848f,	-2.0575f},
//{-38.3404f,	50.1999f,	-32.4776f,	-0.3475f},
//{-0.0017f,	0.0037f,	-0.0025f,	-0.0026f},
//{-2.0087f,	4.4288f,	-2.9594f,	-3.0802f},
//{-40.8246f,	114.0232f,	-106.6627f,	47.4391f},
//{-1.6405f,	15.0387f,	-17.0735f,	8.9177f},
//{944.0681f,	-937.73f,	233.1538f,	58.001f},
//{46.9991f,	41.9957f,	-101.0646f,	51.1905f},
//{0.0497f,	-0.0144f,	-0.027f,	0.0146f},
//{58.7447f,	-16.8466f,	-31.9874f,	17.2433f},
//{1716.4082f,	-2355.0998f,	1185.187f,	-4.1612f},
//{400.3622f,	-544.4472f,	270.2417f,	-0.1013f}

























};

//////////////////////////////////////////////////////////////////*********************************??????????****************************************///////////////////////////////////////////////////////////////



int j=0;
float ROLL_Last[2]={0,0};
float Acc[3];

float yunxing=0;						//???¦Ë
float PianXin=5.5f;
float PITCHPianXin=1.4f;
float xita_dot[2],xita_dot2[2],xita_dot_Last[2];
float L0_dot[2],L0_dot2[2],L0_dot_Last[2];
float L0_Last[2];

uint64_t Time=0;
uint64_t Time_Last=0;
uint8_t Jump_Flag=0;
uint8_t State_Flag=0;
uint8_t State_Last=0;




float Targer_Temp=0;
float ERR_Temp[2];
float err_speed;
float Turn_Tp[2];

void Solve_task(void *pvParameters)
{
	while(1)
	{			
				Time_RUN=Time_Add;
				Time_Delta = Time_RUN-Time_Last;
				Time_Last=Time_RUN;
		
		
				Kalaman_feedback(&Kalman0,(float)Time_Delta/10000.0f,RealSpeed[0],MPU9250_Real_Data.Accel_X*arm_cos_f32(Angular_Handler.ROLL*TheattoRad));
				Kalaman_feedback(&Kalman2,(float)Time_Delta/10000.0f,RealSpeed[1],MPU9250_Real_Data.Accel_X*arm_cos_f32(Angular_Handler.ROLL*TheattoRad));
		
		
		
				
				err_speed=RealSpeed[0]- Kalman0.xhat_data[0];				//???????¦Ä??????????????????????
		
		
				//??????????
				xita1[0] = 3.1415926f+DM[0].Encoder+0.41277036f;/////0jiaodu180   0??????1
				xita4[0] = DM[1].Encoder-0.41277036f;
				xita1[1] = 3.1415926f-DM[3].Encoder+0.41277036f;
				xita4[1] = -DM[2].Encoder-0.41277036f;//3jiadou180
		
		
				
				x1[0] = LEG0*arm_cos_f32(xita1[0]);
				x1[1] = LEG0*arm_cos_f32(xita1[1]);
		
				x2[0] = 0.155f+LEG0*arm_cos_f32(xita4[0]);
				x2[1] = 0.155f+LEG0*arm_cos_f32(xita4[1]);
		
				y1[0] = LEG0*arm_sin_f32(xita1[0]);
				y1[1] = LEG0*arm_sin_f32(xita1[1]);
		
				y2[0] = LEG0 * arm_sin_f32(xita4[0]);
				y2[1] = LEG0 * arm_sin_f32(xita4[1]);
				
				d[0] = __sqrtf((x2[0] - x1[0]) * (x2[0] - x1[0]) + (y2[0] - y1[0]) * (y2[0] - y1[0]));
				d[1] = __sqrtf((x2[1] - x1[1]) * (x2[1] - x1[1]) + (y2[1] - y1[1]) * (y2[1] - y1[1]));
				
				a[0] = d[0] / 2.0f;
				a[1] = d[1] / 2.0f;
				
				h[0] = __sqrtf(LEG1 * LEG1 - a[0] * a[0]);
				h[1] = __sqrtf(LEG1 * LEG1 - a[1] * a[1]);
				
				x3[0] = x1[0] + a[0] * (x2[0] - x1[0]) / d[0];
				x3[1] = x1[1] + a[1] * (x2[1] - x1[1]) / d[1];
				
				y3[0] = y1[0] + a[0] * (y2[0] - y1[0]) / d[0];
				y3[1] = y1[1] + a[1] * (y2[1] - y1[1]) / d[1];
				
				x4_1[0] = x3[0] - h[0] * (y2[0] - y1[0]) / d[0];
				x4_1[1] = x3[1] - h[1] * (y2[1] - y1[1]) / d[1];
				
				y4_1[0] = y3[0] + h[0] * (x2[0] - x1[0]) / d[0];
				y4_1[1] = y3[1] + h[1] * (x2[1] - x1[1]) / d[1];
				
				
				gama[0] = atan2(y4_1[0],(x4_1[0] - 0.0775f));
				gama[1] = atan2(y4_1[1],(x4_1[1] - 0.0775f));
				
				L0_Last[0]=L0[0];
				L0_Last[1]=L0[1];
				
				L0[0] = __sqrtf((x4_1[0]-0.0775f)* (x4_1[0] - 0.0775f)+y4_1[0]*y4_1[0]);																//???????????????gama-90+fai????????????????????????-90
				L0[1] = __sqrtf((x4_1[1]-0.0775f)* (x4_1[1] - 0.0775f)+y4_1[1]*y4_1[1]);
			
				L0_dot_Last[0]=L0_dot[0];
				L0_dot_Last[1]=L0_dot[1];
				
				L0_dot[0]=(L0[0]-L0_Last[0])/0.004f;
				L0_dot[1]=(L0[1]-L0_Last[1])/0.004f;
				
				L0_dot2[0]=(L0_dot[0]-L0_dot_Last[0])/0.004f;
				L0_dot2[1]=(L0_dot[1]-L0_dot_Last[1])/0.004f;
			
			
			
			
				xita_Last[0]=xita[0];
				xita_Last[1]=xita[1];
				
				
				xita[0]=	(-Angular_Handler.ROLL+PianXin+90-gama[0]*RadtoTheat)*TheattoRad;			//?????????????????????????gama???§³??90?????????????????????????		
				xita[1]=	(-Angular_Handler.ROLL+PianXin+90-gama[1]*RadtoTheat)*TheattoRad;
				
				xita_dot_Last[0]=xita_dot[0];
				xita_dot_Last[1]=xita_dot[1];
				
				xita_dot[0]=(xita[0]-xita_Last[0])/0.004f;
				xita_dot[1]=(xita[1]-xita_Last[1])/0.004f;
				
				xita_dot2[0]=(xita_dot[0]-xita_dot_Last[0])/0.004f;
				xita_dot2[1]=(xita_dot[1]-xita_dot_Last[1])/0.004f;
				
				
				
				
				
				
				
				
				FT[0]		=(arm_sin_f32(xita2[0] - xita3[0])*(-DM[1].Tor*LEG0*arm_cos_f32(gama[0] - xita3[0])*arm_sin_f32(xita1[0] - xita2[0]) +DM[0].Tor*LEG0*arm_cos_f32(gama[0] - xita2[0])*arm_sin_f32(xita3[0] - xita4[0])))/(LEG0*LEG0*arm_sin_f32(xita1[0] - xita2[0])*arm_sin_f32(xita3[0] - xita4[0])*(arm_cos_f32(gama[0] - xita2[0])*arm_sin_f32(gama[0] - xita3[0]) - arm_cos_f32(gama[0] - xita3[0])*arm_sin_f32(gama[0] - xita2[0])));
				FTTP[0]	=-(arm_sin_f32(xita2[0] - xita3[0])*(-DM[1].Tor*LEG0*arm_sin_f32(gama[0] - xita3[0])*arm_sin_f32(xita1[0] - xita2[0]) + DM[0].Tor*LEG0*arm_sin_f32(gama[0] - xita2[0])*arm_sin_f32(xita3[0] - xita4[0])))/(LEG0*LEG0*arm_sin_f32(xita1[0] - xita2[0])*arm_sin_f32(xita3[0] - xita4[0])*(arm_cos_f32(gama[0] - xita2[0])*arm_sin_f32(gama[0] - xita3[0]) - arm_cos_f32(gama[0]- xita3[0])*arm_sin_f32(gama[0] - xita2[0])));
				
				
				FT[1]		=(arm_sin_f32(xita2[1] - xita3[1])*(-DM[2].Tor*LEG0*arm_cos_f32(gama[1] - xita3[1])*arm_sin_f32(xita1[1] - xita2[1]) + DM[3].Tor*LEG0*arm_cos_f32(gama[1] - xita2[1])*arm_sin_f32(xita3[1] - xita4[1])))/(LEG0*LEG0*arm_sin_f32(xita1[1] - xita2[1])*arm_sin_f32(xita3[1] - xita4[1])*(arm_cos_f32(gama[1] - xita2[1])*arm_sin_f32(gama[1] - xita3[1]) - arm_cos_f32(gama[1] - xita3[1])*arm_sin_f32(gama[1] - xita2[1])));
				FTTP[1]	=-(arm_sin_f32(xita2[1] - xita3[1])*(-DM[2].Tor*LEG0*arm_sin_f32(gama[1] - xita3[1])*arm_sin_f32(xita1[1] - xita2[1]) + DM[3].Tor*LEG0*arm_sin_f32(gama[1] - xita2[1])*arm_sin_f32(xita3[1] - xita4[1])))/(LEG0*LEG0*arm_sin_f32(xita1[1] - xita2[1])*arm_sin_f32(xita3[1] - xita4[1])*(arm_cos_f32(gama[1] - xita2[1])*arm_sin_f32(gama[1] - xita3[1]) - arm_cos_f32(gama[1]- xita3[1])*arm_sin_f32(gama[1] - xita2[1])));
//				
//			FT[1]		=FTC[1];
//			FTTP[1]	=FTT[1]*(-L0[1]);

				FP[0]=FT[0]*arm_cos_f32(xita[0])+FTTP[0]*arm_sin_f32(xita[0])*L0[0];
				FP[1]=FT[1]*arm_cos_f32(xita[1])+FTTP[1]*arm_sin_f32(xita[1])*L0[1];
				

						
				Acc[0]=MPU9250_Real_Data.Accel_Z*arm_cos_f32(Angular_Handler.Pitch*TheattoRad)*arm_cos_f32(Angular_Handler.ROLL*TheattoRad)+MPU9250_Real_Data.Accel_Y*arm_cos_f32(Angular_Handler.Pitch*TheattoRad)*arm_sin_f32(Angular_Handler.ROLL*TheattoRad)+MPU9250_Real_Data.Accel_X*arm_sin_f32(Angular_Handler.Pitch*TheattoRad)*arm_cos_f32(Angular_Handler.ROLL*TheattoRad);
				Acc[1]=Acc[0]-5.0f-L0_dot2[0]*arm_cos_f32(xita[0])+2*L0_dot[0]*xita_dot[0]+L0[0]*xita_dot[0]*xita_dot[0]*arm_cos_f32(xita[0])+L0[0]*xita_dot2[0]*arm_sin_f32(xita[0]);
				Acc[2]=Acc[0]-5.0f-L0_dot2[1]*arm_cos_f32(xita[1])+2*L0_dot[1]*xita_dot[1]+L0[1]*xita_dot[1]*xita_dot[1]*arm_cos_f32(xita[1])+L0[1]*xita_dot2[1]*arm_sin_f32(xita[1]);

				
//				
				FN[0]=-FP[0]+15.2f*9.8f-0.02f*Acc[1];
				FN[1]=FP[1]+15.2f*9.8f-0.02f*Acc[2];
					
				
				
				
/////////////////////////////////////////////////////////////////////??????????????////////////////////////////////				
					
				if((FN[0]<195&&FN[1]<195)&&!Tim_Flag&&LEG_Flag)
				{
					speed_zero=0;
					Zero=0;
					Goal_y[0] =0.20f-0.20f*arm_sin_f32((Angular_Handler.Pitch-PITCHPianXin)*TheattoRad)+rc_ctrl.rc.ch[2]/1320.0f*0.46f;//+0.125f*arm_sin_f32((Angular_Handler.Pitch+1.8f)*TheattoRad);
					Goal_y[1] =0.20f+0.20f*arm_sin_f32((Angular_Handler.Pitch-PITCHPianXin)*TheattoRad)+rc_ctrl.rc.ch[2]/1320.0f*0.46f;//-0.125f*arm_sin_f32((Angular_Handler.Pitch+1.8f)*TheattoRad);	
					Limit_Min_Max(Goal_y[0],0.15f,0.45f);
					Limit_Min_Max(Goal_y[1],0.15f,0.45f);
					yunxing=0;

				}
				else
				{
					yunxing=250;
					Zero=1.0f;
			
					Goal_y[0] =0.20f-0.20f*arm_sin_f32((Angular_Handler.Pitch-PITCHPianXin)*TheattoRad)+rc_ctrl.rc.ch[2]/1320.0f*0.46f;//+0.125f*arm_sin_f32((Angular_Handler.Pitch+1.8f)*TheattoRad);
					Goal_y[1] =0.20f+0.20f*arm_sin_f32((Angular_Handler.Pitch-PITCHPianXin)*TheattoRad)+rc_ctrl.rc.ch[2]/1320.0f*0.46f;//-0.125f*arm_sin_f32((Angular_Handler.Pitch+1.8f)*TheattoRad)
					Limit_Min_Max(Goal_y[0],0.15f,0.45f);
					Limit_Min_Max(Goal_y[1],0.15f,0.45f);
				}
					

				
				



/////////////////////////////////////////////////////////////////////LQR?????????////////////////////////////////				
	
				for(int m=0;m<2;m++)
				{
					for(j=0;j<12;j++)
					{
						Ks[m][j]=Ka[j][0]*L0[m]*L0[m]*L0[m]+Ka[j][1]*L0[m]*L0[m]+Ka[j][2]*L0[m]+Ka[j][3];
					}
				}
				
				Targer_Temp=RAMP_float(((float)rc_ctrl.rc.ch[1]+(float)(FLAG.Key_State.W-FLAG.Key_State.S)*330.0f)/660.0f*2.5f,Targer_Temp,0.07272f);
				
				
				//???????????????????????
				ERR_Temp[0]= Kalman2.xhat_data[0]*Dodge_Speed;
				ERR_Temp[1]=-Kalman0.xhat_data[0]*Dodge_Speed;

				
				//u0??????? xita0???????????????	????xita1??????? ???0????????????
				//Limit_Min_Max(Targer_Temp,ERR_Temp[0]-1.0f,ERR_Temp[0]+1.0f);
				//Limit_Min_Max(Targer_Temp,ERR_Temp[1]-1.0f,ERR_Temp[1]+1.0f);
				
				
				u[0]=(1.0f*Ks[0][0]*xita[0]+Ks[0][1]*(xita[0]-xita_Last[0])/0.006f+	  Ks[0][3]*(1.0f*ERR_Temp[0]-Targer_Temp)+1.2f*Ks[0][4] *(Angular_Handler.ROLL-PianXin)*TheattoRad+Ks[0][5] *(Angular_Handler.ROLL-ROLL_Last[0])/0.020f*TheattoRad)*Zero;
				u[1]= 1.0f*Ks[0][6]*xita[0]+Ks[0][7]*(xita[0]-xita_Last[0])/0.006f+Zero*(Ks[0][9]*(1.0f*ERR_Temp[0]-Targer_Temp)+1.2f*Ks[0][10]*(Angular_Handler.ROLL-PianXin)*TheattoRad+Ks[0][11]*(Angular_Handler.ROLL-ROLL_Last[0])/0.020f*TheattoRad);
				
				
				//??????????????????//?????????§³????			//?????zuo???1??you???0   ????????? ????????
				
				
				u[2]=(1.0f*Ks[1][0]*xita[1]+Ks[1][1]*(xita[1]-xita_Last[1])/0.006f+	 	Ks[1][3]*(1.0f*ERR_Temp[1]-Targer_Temp)+1.2f*Ks[1][4] *(Angular_Handler.ROLL-PianXin)*TheattoRad+Ks[1][5] *(Angular_Handler.ROLL-ROLL_Last[1])/0.020f*TheattoRad)*Zero;
				u[3]= 1.0f*Ks[1][6]*xita[1]+Ks[1][7]*(xita[1]-xita_Last[1])/0.006f+Zero*(Ks[1][9]*(1.0f*ERR_Temp[1]-Targer_Temp)+1.2f*Ks[1][10]*(Angular_Handler.ROLL-PianXin)*TheattoRad+Ks[1][11]*(Angular_Handler.ROLL-ROLL_Last[1])/0.020f*TheattoRad);

				
				
				ROLL_Last[0]=Angular_Handler.ROLL;
				ROLL_Last[1]=Angular_Handler.ROLL;
				
				xita2[0] = atan2((y4_1[0] - y1[0]), (x4_1[0] - x1[0]));
				xita2[1] = atan2((y4_1[1] - y1[1]), (x4_1[1] - x1[1]));
				
				xita3[0] = atan2((y4_1[0]-y2[0]),(x4_1[0]-x2[0]));
				xita3[1] = atan2((y4_1[1]-y2[1]),(x4_1[1]-x2[1]));
				
				e[0] = LEG0 * arm_sin_f32(gama[0] - xita3[0]) * arm_sin_f32(xita1[0] - xita2[0]) / arm_sin_f32(xita3[0] - xita2[0]);
				f[0] = LEG0 * arm_cos_f32(gama[0] - xita3[0]) * arm_sin_f32(xita1[0] - xita2[0]) / (L0[0]*arm_sin_f32(xita3[0] - xita2[0]));
				c[0] = LEG0 * arm_sin_f32(gama[0] - xita2[0]) * arm_sin_f32(xita3[0] - xita4[0]) / arm_sin_f32(xita3[0] - xita2[0]);
				g[0] = LEG0 * arm_cos_f32(gama[0] - xita2[0]) * arm_sin_f32(xita3[0] - xita4[0]) / (L0[0]*arm_sin_f32(xita3[0] - xita2[0]));
				
				e[1] = LEG0 * arm_sin_f32(gama[1] - xita3[1]) * arm_sin_f32(xita1[1] - xita2[1]) / arm_sin_f32(xita3[1] - xita2[1]);
				f[1] = LEG0 * arm_cos_f32(gama[1] - xita3[1]) * arm_sin_f32(xita1[1] - xita2[1]) / (L0[1]*arm_sin_f32(xita3[1] - xita2[1]));
				c[1] = LEG0 * arm_sin_f32(gama[1] - xita2[1]) * arm_sin_f32(xita3[1] - xita4[1]) / arm_sin_f32(xita3[1] - xita2[1]);
				g[1] = LEG0 * arm_cos_f32(gama[1] - xita2[1]) * arm_sin_f32(xita3[1] - xita4[1]) / (L0[1]*arm_sin_f32(xita3[1] - xita2[1]));
			
				J[0]= LEG0 * arm_sin_f32(xita3[0]) * arm_sin_f32(xita1[0] - xita2[0]) / arm_sin_f32(xita2[0] - xita3[0]);
				J[1]=-LEG0 * arm_cos_f32(xita3[0]) * arm_sin_f32(xita1[0] - xita2[0]) / arm_sin_f32(xita2[0] - xita3[0]);
				J[2]= LEG0 * arm_sin_f32(xita2[0]) * arm_sin_f32(xita3[0] - xita4[0]) / arm_sin_f32(xita2[0] - xita3[0]);
				J[3]=-LEG0 * arm_cos_f32(xita2[0]) * arm_sin_f32(xita3[0] - xita4[0]) / arm_sin_f32(xita2[0] - xita3[0]);
				
				J[4]= LEG0 * arm_sin_f32(xita3[1]) * arm_sin_f32(xita1[1] - xita2[1]) / arm_sin_f32(xita2[1] - xita3[1]);
				J[5]=-LEG0 * arm_cos_f32(xita3[1]) * arm_sin_f32(xita1[1] - xita2[1]) / arm_sin_f32(xita2[1] - xita3[1]);
				J[6]= LEG0 * arm_sin_f32(xita2[1]) * arm_sin_f32(xita3[1] - xita4[1]) / arm_sin_f32(xita2[1] - xita3[1]);
				J[7]=-LEG0 * arm_cos_f32(xita2[1]) * arm_sin_f32(xita3[1] - xita4[1]) / arm_sin_f32(xita2[1] - xita3[1]);
				
				
				real_y[0]=L0[0]*arm_cos_f32(xita[0]);
				real_y[1]=L0[1]*arm_cos_f32(xita[1]);
				
				
				real_x[0]=L0[0]*arm_sin_f32(xita[0])+0.0775f;
				real_x[1]=L0[1]*arm_sin_f32(xita[1])+0.0775f;
			
				
				
				
				
				Err_or[0]=(Goal_y[0]-real_y[0]);
				Err_or[1]=(Goal_y[1]-real_y[1]);
				
				Err_or[2]=(Goal_x[0]-real_x[0]);
				Err_or[3]=(Goal_x[1]-real_x[1]);
				
				
				F[0]=Kp_Leg[0]*(Err_or[0])+F_front[0]+Kd_Leg[0]*100.0f*(Err_or[0]-Err_Last_or[0])-120.0f*((Angular_Handler.Pitch-PITCHPianXin)*TheattoRad)*Zero*Dodge_Speed-2500.0f*((Angular_Handler.Pitch-Pitch_Last[0])*TheattoRad)*Zero;
				F[1]=Kp_Leg[2]*(Err_or[1])+F_front[1]+Kd_Leg[2]*100.0f*(Err_or[1]-Err_Last_or[1])+120.0f*((Angular_Handler.Pitch-PITCHPianXin)*TheattoRad)*Zero*Dodge_Speed+2500.0f*((Angular_Handler.Pitch-Pitch_Last[1])*TheattoRad)*Zero;
//				Fy[0]=0.8f*Fy[0]+0.2f*Fy_Last[0];
//				Fy[1]=0.8f*Fy[1]+0.2f*Fy_Last[1];
				
				Fy_Last[0]=Fy[0];
				Fy_Last[1]=Fy[1];
				
//		
//				F[0]=-0.0f*((Angular_Handler.Pitch+0.6f)*TheattoRad)*Zero*Dodge_Speed-0.0f*((-Angular_Handler.Pitch-Pitch_Last[0])*TheattoRad)*Zero;
//				F[1]=+0.0f*((Angular_Handler.Pitch+0.6f)*TheattoRad)*Zero*Dodge_Speed+0.0f*((-Angular_Handler.Pitch-Pitch_Last[1])*TheattoRad)*Zero;
//				
				
				Pitch_Last[0]=Angular_Handler.Pitch;
				Pitch_Last[1]=Angular_Handler.Pitch;
				
				
//				Fx[0]=(Kp_Leg[1]*(Err_or[2])+Kd_Leg[1]*100.0f*(Err_or[2]-Err_Last_or[2]));
//				Fx[1]=(Kp_Leg[3]*(Err_or[3])+Kd_Leg[3]*100.0f*(Err_or[3]-Err_Last_or[3]));
				
				
			
				Err_Last_or[0]=Goal_y[0]-real_y[0];
				Err_Last_or[1]=Goal_y[1]-real_y[1];
				
				Err_Last_or[2]=Goal_x[0]-real_x[0];				
				Err_Last_or[3]=Goal_x[1]-real_x[1];
				

				
				
				
				Turn_Tp[0]=-(xita[0]-xita[1])*kd_Turn[0]*RadtoTheat;
				Turn_Tp[1]=-(xita[0]-xita[1])*kd_Turn[1]*RadtoTheat;
				Limit_Min_Max(Turn_Tp[0],-8,8);
				Limit_Min_Max(Turn_Tp[1],-8,8);
				Tp[0] = u[1]+(Turn_Tp[0])*Zero;
				Tp[1] = u[3]-(Turn_Tp[1])*Zero;
								
				T1[0]= (e[0] * F[0] + f[0]*Tp[0] +J[0]*Fx[0] + J[1]*Fy[0]);
				T2[0]= (c[0] * F[0] + g[0]*Tp[0] +J[2]*Fx[0] + J[3]*Fy[0]);
				
				T1[1]= (e[1] * F[1] + f[1]*Tp[1] +J[4]*Fx[1] + J[5]*Fy[1]);
				T2[1]= (c[1] * F[1] + g[1]*Tp[1] +J[6]*Fx[1] + J[7]*Fy[1]);
							


				
				TTOR[0]=T1[0];
				TTOR[1]=T2[0];
				
				TTOR[2]=-T2[1];
				TTOR[3]=-T1[1];
				
			
				if(RC_Mode==CHASSIS_RELAX)
				{
					for(int i=0;i<4;i++)
					{
						Can_DM8009_DISABLE(i,0);
						vTaskDelay(1);
					}
				}
				else if(RC_Mode==CHASSIS_INIT)
				{	
					LEG_Flag=0;; 
					Zero=1.0f;
					speed_zero=1;
					F_front[0]=	 95.5f;
					F_front[1]=	 95.5f;
					Kp_Leg[0] =    900;
					Kp_Leg[2] =    900;
					Kd_Leg[0] =    150;
					Kd_Leg[2] =    150;
					Goal_y[0] =0.20f;//+0.100f*arm_sin_f32((Angular_Handler.Pitch+0.4f)*TheattoRad);//-rc_ctrl.rc.ch[2]/1320.0f*0.18f;//
					Goal_y[1] =0.20f;//-0.100f*arm_sin_f32((Angular_Handler.Pitch+0.4f)*TheattoRad);//+rc_ctrl.rc.ch[2]/1320.0f*0.18f;//
					//???PID????????????????????
					
					Tim_Flag=1;
					for(int i=0;i<4;i++)
					{
						Can_DM8009_TOR(i,0);
						vTaskDelay(1);
					}
					for(int i=0;i<4;i++)
					{
						Can_DM8009_ENABLE(i,0);
						vTaskDelay(1);
					}
				
					
				}
				else if(RC_Mode==CHASSIS_FOLLOW_GIMBAL||RC_Mode==CHASSIS_DODGE_MODE)
				{	
					if(fabs(Angular_Handler.ROLL-PianXin)<3.0f)
					{
						
						LEG_Flag=1;
						
					}
					
					
					if(FN[0]>200&&FN[1]>200&&Tim_Flag&&LEG_Flag)
					{
						Tim_Flag=0;		
					}
					if(LEG_Flag)
					{
						walkkp=193.9393f;
						for(int i=0;i<4;i++)
						{
							Limit_Min_Max(TTOR[i],-30,30);
							Can_DM8009_TOR(i,TTOR[i]);					///??§Õ???§Û???????//?????0????????TTOR[i]
							vTaskDelay(1);
						}
					}
					else
					{
						walkkp=90.9393f;
						vTaskDelay(1);
					
					}
				}
				else if(RC_Mode==CHASSIS_LEG_INIT_MODE)
				{	
					for(int i=0;i<4;i++)
					{
						Can_DM8009_ZERO(i,0);
						vTaskDelay(1);
					}
				}
				else
				{
	
					for(int i=0;i<4;i++)
					{
						Can_DM8009_TOR(i,0);
						vTaskDelay(1);
					}
				
				}
					
				
				Super_power_ctrl();
	}
}




static void Super_power_ctrl(void)
{
	uint16_t fTotalpowerLimit;

	if (GameState.game_type==0)
	{
		CAN_CMD_SUPERPOWER(40, Shift_Flag, 0); // 20W??????
	}
	else if(GameState.game_type==1)
	{
		if(GameState.game_progress==4)
		{
			fTotalpowerLimit = GameRobotStat.chassis_power_limit;
			CAN_CMD_SUPERPOWER(fTotalpowerLimit, Shift_Flag, 60); //?§Ý????
		}
		else if(GameState.game_progress==1||GameState.game_progress==2||GameState.game_progress==3)
		{
			CAN_CMD_SUPERPOWER(0,Shift_Flag,0); //?§Ý????
		}
	}
}

void chassis_mode_set(void)
{
				//??????????????   ????S????????????????
    if(switch_is_mid(rc_ctrl.rc.s[1])&&switch_is_up(rc_ctrl.rc.s[0]))//???????? PC?????
		{  
			
			if(!IF_KEY_PRESSED_E && !IF_KEY_PRESSED_CTRL)
			{
				Mouse_ZU = 1;
			}
			if(!IF_KEY_PRESSED_E)//????F??????
			{
				E_Flag = 1;
			}
			else
			{
			}

			if(IF_KEY_PRESSED_E && Mouse_ZU == 1 && IF_KEY_PRESSED_CTRL)
			{
				E_Flag = 0;
				Mouse_ZU = 0;
				Cutting++;
				Cutting %= 3;
			}
			else if (IF_KEY_PRESSED_E && E_Flag == 1)
			{
				E_Flag = 0;
				EFlag_state ++;
				EFlag_state %= 2;
			}
			else
			{
				
				
			}
			
			
			
			if(IF_KEY_PRESSED_F==0)//????F??????
			{
				F_Flag = 1;
			}
			if (IF_KEY_PRESSED_F&& F_Flag == 1)
			{
				F_Flag = 0;
				FFlag_state ++;
				FFlag_state %= 2;
			}
			if(FFlag_state == 0) 
			{
				RC_Mode = CHASSIS_FOLLOW_GIMBAL;
			}
			else if(FFlag_state)		//§³??????	
			{
				RC_Mode = CHASSIS_DODGE_MODE; 
			}		
			if((!(rc_ctrl.key.v & KEY_PRESSED_OFFSET_SHIFT))!= 0)
			{				
				Shift_Flag=2;
			}
			if ((rc_ctrl.key.v & KEY_PRESSED_OFFSET_SHIFT)&&(Super_power.volt > 12000))
			{
				Shift_Flag = 1;
			}
			else 
			{
				Shift_Flag = 2;
			}
			
			if(IF_KEY_PRESSED_R==0)//????R??????
			{
				R_Flag = 1;
			}
			if (IF_KEY_PRESSED_R && R_Flag == 1)
			{
				R_Flag = 0;
				RFlag_state ++;
				RFlag_state %= 2;
			}
			if(IF_KEY_PRESSED_G==0)//????R??????
			{
				G_Flag = 1;
			}
			if (IF_KEY_PRESSED_G && G_Flag == 1)
			{
				G_Flag = 0;
				GFlag_state ++;
				GFlag_state %= 3;
			}	
			if(IF_KEY_PRESSED_V==0)//????V??????
			{
				V_Flag = 1;
			}	
			if (IF_KEY_PRESSED_V && V_Flag == 1)
			{
				V_Flag = 0;
				VFlag_state ++;
				VFlag_state %= 2;
			}
			if(IF_KEY_PRESSED_Z==0)//????Z??????
			{
				Z_Flag = 1;
			}
			if (IF_KEY_PRESSED_Z && Z_Flag == 1)
			{
				Z_Flag = 0;
				ZFlag_state ++;
				ZFlag_state %= 2;
			}
			if(IF_KEY_PRESSED_C==0)//????C??????
			{
				C_Flag = 1;
			}
			if (IF_KEY_PRESSED_C && C_Flag == 1)
			{
				C_Flag = 0;
				CFlag_state ++;
				CFlag_state %= 2;
			}
			if(IF_KEY_PRESSED_B==0)//????B??????
			{
				B_Flag = 1;
			}
			if (IF_KEY_PRESSED_B && B_Flag == 1)
			{
				B_Flag = 0;
				BFlag_state ++;
				BFlag_state %= 2;
			}
			if(IF_KEY_PRESSED_X==0)//????X??????
			{
				X_Flag = 1;
			}	
			if (IF_KEY_PRESSED_X && X_Flag == 1)
			{
				X_Flag = 0;
				XFlag_state ++;
				XFlag_state %= 2;
			}
			FLAG.Key_State.W=IF_KEY_PRESSED_W;
			FLAG.Key_State.A=IF_KEY_PRESSED_A;
			FLAG.Key_State.S=IF_KEY_PRESSED_S;
			FLAG.Key_State.D=IF_KEY_PRESSED_D;
			FLAG.Key_State.B=BFlag_state;
			FLAG.Key_State.C=CFlag_state;
			FLAG.Key_State.F=FFlag_state;
			FLAG.Key_State.G=GFlag_state;
			FLAG.Key_State.Q=IF_KEY_PRESSED_Q;
			FLAG.Key_State.R=RFlag_state;
			FLAG.Key_State.V=IF_KEY_PRESSED_V;
			FLAG.Key_State.X=XFlag_state;
			FLAG.Key_State.Z=ZFlag_state;
			FLAG.Key_State.CRTL=IF_KEY_PRESSED_CTRL;	
			FLAG.Key_State.E=EFlag_state;
		}
		else
		{
			
			if (switch_is_down(rc_ctrl.rc.s[1])&&switch_is_down(rc_ctrl.rc.s[0]))   //????????  ????
			{
					RC_Mode = CHASSIS_RELAX ;
			}
			else if (switch_is_down(rc_ctrl.rc.s[1])&&switch_is_mid(rc_ctrl.rc.s[0]))  //???????? ?????
			{
					RC_Mode = CHASSIS_INIT;
			}
			else if(switch_is_mid(rc_ctrl.rc.s[1])&&switch_is_mid(rc_ctrl.rc.s[0]))		//?????????§µ????????
			{
				  RC_Mode =	CHASSIS_FOLLOW_GIMBAL;
			}
			else if(switch_is_up(rc_ctrl.rc.s[1])&&switch_is_mid(rc_ctrl.rc.s[0]))		//?????????§µ?§³????
			{
					RC_Mode = CHASSIS_DODGE_MODE;
			}
			else if(switch_is_mid(rc_ctrl.rc.s[1]) && switch_is_down(rc_ctrl.rc.s[0]))
			{
					RC_Mode = CHASSIS_LEG_INIT_MODE;
			}
			else
			{
					RC_Mode = CHASSIS_RELAX;
			}
		}	
}









