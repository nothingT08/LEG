#ifndef __Variable_H_
#define __Variable_H_

#include "sys.h"
#include "stm32f4xx.h" 
#include "arm_math.h"


//此文件用于各类参数的统一初始化以及各种宏定义的写入，理想状态在各文件中无需变量命名，统一在此命名，因为时间问题个文件中存在变量未写入该文件





//矩阵向右旋转角度xita为正
#define  pai 											 3.1415926f

#define  RadtoTheat 							 180.f/pai
#define  TheattoRad  							 pai/180.f
#define  CAN_3508_ALL_ID					 0x200
#define  CAN_6020_ALL_ID 					 0x1FF
#define  WARNING_REMAIN_POWER 		 60

#define  CHASSIS_RELAX           	  0//底盘无力模式
#define  CHASSIS_INIT           	  1
#define  CHASSIS_FOLLOW_GIMBAL  	  2//自动跟随云台
#define  CHASSIS_SEPARATE_GIMBAL    3//云台分离
#define  CHASSIS_DODGE_MODE         4//小陀螺
#define  CHASSIS_LEG_INIT_MODE      5//腿部初始化


#define mat arm_matrix_instance_f32
#define Matrix_Init arm_mat_init_f32
#define Matrix_Add arm_mat_add_f32
#define Matrix_Subtract arm_mat_sub_f32
#define Matrix_Multiply arm_mat_mult_f32
#define Matrix_Transpose arm_mat_trans_f32
#define Matrix_Inverse arm_mat_inverse_f32





#define  Limit_Min_Max(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))


#define 			pai 3.1415926f
#define       YawMedian    		600
#define       PitchMedian   	6780//4100
#define       PitchMin				6000//3400
#define       PitchMax				8000//4600

#define 			BLUE  					0
#define 			RED   					1

#define 			START_TASK_PRIO						1
#define 			START_STK_SIZE 						512  


#define 			IMU_TASK_PRIO							9
#define 			IMU_STK_SIZE 							512

#define 			JUDGE_TASK_PRIO 					4
#define 			JUDGE_TASK_SIZE 					256

#define 			UI_TASK_PRIO 							3
#define 			UI_TASK_SIZE 							1024

#define 			Solve_TASK_PRIO						7
#define 			Solve_STK_SIZE 						2048

#define 			CHASSIS_TASK_PRIO					5
#define 			CHASSIS_STK_SIZE 					1024  


#define 			P_MIN						-pai
#define 			P_MAX 					pai

#define 			V_MIN						-45
#define 			V_MAX 					45  

#define 			T_MIN						-54
#define 			T_MAX 					54  




#pragma pack(1)
typedef struct
{
    uint8_t id     :4;      // 电机ID: 0,1...,13,14 15表示向所有电机广播数据(此时无返回)
    uint8_t status :3;      // 工作模式: 0.锁定 1.FOC闭环 2.编码器校准 3.保留
    uint8_t none   :1;      // 保留位
} SIS_Mode_t;   // 控制模式 1Byte

typedef struct
{
    uint8_t id     :4;      // 电机ID: 0,1...,13,14 15表示向所有电机广播数据(此时无返回)
    uint8_t status :3;      // 工作模式: 0.锁定 1.FOC闭环 2.编码器校准 3.保留
    uint8_t none   :1;      // 保留位
} RIS_Mode_t;   // 控制模式 1Byte


typedef struct
{
    int16_t tor_des;        // 期望关节输出扭矩 unit: N.m      (q8)
    int16_t spd_des;        // 期望关节输出速度 unit: rad/s    (q8)
    int32_t pos_des;        // 期望关节输出位置 unit: rad      (q15)
    int16_t k_pos;          // 期望关节刚度系数 unit: -1.0-1.0 (q15)
    int16_t k_spd;          // 期望关节阻尼系数 unit: -1.0-1.0 (q15)
    
} SIS_Comd_t;   // 控制参数 12Byte


typedef struct
{
	int16_t MERR:3;
	int16_t Force:12;
	uint16_t Re:1;

}ERR;

typedef struct
{
    int16_t  tor;        // 期望关节输出扭矩 unit: N.m      (q8)
    int16_t  Speed;        // 期望关节输出速度 unit: rad/s    (q8)
    int32_t  Pos;        // 期望关节输出位置 unit: rad      (q15)
    int8_t 	 Temp;          // 期望关节刚度系数 unit: -1.0-1.0 (q15)
    ERR      ERRFOR;          // 期望关节阻尼系数 unit: -1.0-1.0 (q15)
    
} RIS_Comd_t;   // 控制参数 12Byte




typedef struct
{
    // 定义 电机控制命令数据包
    uint8_t head[2];    // 包头         2Byte
    SIS_Mode_t mode;    // 电机控制模式  1Byte
    SIS_Comd_t comd;    // 电机期望数据 12Byte
    uint16_t   CRC16;   // CRC          2Byte
} ControlData_t;     //电机控制命令数据包

typedef struct
{
    // 定义 电机控制命令数据包
    uint8_t head[2];    // 包头         2Byte
    RIS_Mode_t mode;    // 电机控制模式  1Byte
    RIS_Comd_t comd;    // 电机期望数据 12Byte
    uint16_t   CRC16;   // CRC          2Byte
} ReceData_t;     //电机控制命令数据包

typedef union 
{
	ControlData_t motor_send_data;  
	uint8_t Data[17];

}Send;

#pragma pack()

typedef struct
{
	uint8_t ERR;								//电机温度
	float Encoder;									//电机转速
	float SPEED;								//编码器位置
	float Tor;											//转矩电流
	int8_t  T_MOS;
	int8_t  T_Rotor;

}DM8009;



typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float err;
	float err_last;
	float	actual;
	float integral;
	float target;

}PID;


typedef struct	
{
    uint8_t angle[2];
    uint8_t rpm[2];
    uint8_t torque[2];
    uint8_t null[2];
		int CircleNUM;
		int16_t angleLast;
		float  RealAngle;
		int16_t Realrpm;
		int16_t Angle;
		int16_t FI;
}motor;


typedef struct
{
	unsigned W:1;
	unsigned A:1;
	unsigned S:1;
	unsigned D:1;
	unsigned Q:1;
	unsigned E:1;
	unsigned G:1;
	unsigned X:1;
	unsigned Z:1;
	unsigned C:1;
	unsigned B:1;
	unsigned V:1;
	unsigned F:1;
	unsigned R:1;
	unsigned CRTL:1;
	unsigned SHIFT:1;

}KEY;


typedef union
{
	KEY Key_State;
	uint8_t RX[2];
}KEY_STATE;


typedef struct
{
    uint16_t 	volt;
    uint16_t	power;
    uint16_t	current;
} Super_power_t;

typedef struct 
{
  int16_t  power;
	int16_t  i;
	float   buffer_power;
}Send_data;

typedef union 
{
   Send_data  TX_Data;
   uint8_t  TX_Data_char[sizeof(Send_data)];
}TX_Union_data;

typedef struct
{
	uint8_t Motor_Temp;								//电机温度
	uint8_t SPEED[2];									//电机转速
	uint8_t Encoder[2];								//编码器位置
	uint8_t Iq[2];											//转矩电流
	int16_t RealSpeed;
	uint16_t RealEncoder;
	int16_t RealIq;
	uint16_t EncoderLast;
	int16_t CircleNUM;

}MF9025;




extern uint8_t BFlag_state;
extern uint8_t VFlag_state;
extern uint8_t ZFlag_state;
extern uint8_t XFlag_state;
extern uint8_t CFlag_state;
extern uint8_t RFlag_state;
extern uint8_t FFlag_state;	
extern uint8_t CTRL_State ;
extern uint8_t GFlag_state;
extern uint8_t MouseZU_State;
extern uint8_t MouseZB_State;

extern uint8_t L_Data;
extern uint8_t BUFF_DMA[64];

extern uint8_t Flag;
extern uint8_t RID;

extern KEY_STATE FLAG;

extern PID Turn[8];


extern int Cutting;
extern float RealSpeed[2];

extern float YAW_CTRL;
extern float YAW_CTRL_Real;

extern Super_power_t Super_power;

extern int 		RC_Mode;
extern int 		SHIFT_PRESS;
extern int    Mode;
extern int 		Last_Mode;


extern MF9025 MF[2];
extern motor DJ6020[4];
extern uint8_t RS485_RX_CNT;
extern float Distan;
extern DM8009 DM[4];
float uint_to_float(int x_int,float x_min,float x_max,int bits);
int	  float_to_uint(float x,float x_min,float x_max,int bits);
float RAMP_float( float final, float now, float ramp );




struct kf_t;
typedef struct kf_t
{
    float *FilteredValue;
    float *MeasuredVector;
    float *ControlVector;

    uint8_t xhatSize;
    uint8_t uSize;
    uint8_t zSize;

    uint8_t UseAutoAdjustment;
    uint8_t MeasurementValidNum;

    uint8_t *MeasurementMap;      // 量测与状态的关系 how measurement relates to the state
    float *MeasurementDegree;     // 测量值对应H矩阵元素值 elements of each measurement in H
    float *MatR_DiagonalElements; // 量测方差 variance for each measurement
    float *StateMinVariance;      // 最小方差 避免方差过度收敛               suppress filter excessive convergence
    uint8_t *temp;

    // 配合用户定义函数使用,作为标志位用于判断是否要跳过标准KF中五个环节中的任意一个
    uint8_t SkipEq1, SkipEq2, SkipEq3, SkipEq4, SkipEq5;

    // definiion of struct mat: rows & cols & pointer to vars
    mat xhat;      // x(k|k)
    mat xhatminus; // x(k|k-1)
    mat u;         // control vector u
    mat z;         // measurement vector z
    mat P;         // covariance matrix P(k|k)
    mat Pminus;    // covariance matrix P(k|k-1)
    mat F, FT,temp_F;     // state transition matrix F FT
    mat B;         // control matrix B
    mat H, HT;     // measurement matrix H
    mat Q;         // process noise covariance matrix Q
    mat R;         // measurement noise covariance matrix R
    mat K;         // kalman gain  K
    mat S, temp_matrix, temp_matrix1, temp_vector, temp_vector1;

    int8_t MatStatus;

    // 用户定义函数,可以替换或扩展基准KF的功能
    void (*User_Func0_f)(struct kf_t *kf);
    void (*User_Func1_f)(struct kf_t *kf);
    void (*User_Func2_f)(struct kf_t *kf);
    void (*User_Func3_f)(struct kf_t *kf);
    void (*User_Func4_f)(struct kf_t *kf);
    void (*User_Func5_f)(struct kf_t *kf);
    void (*User_Func6_f)(struct kf_t *kf);
    
    // 矩阵存储空间指针
    float xhat_data[2], xhatminus_data[2];
    float u_data[4];
    float z_data[2];
    float P_data[4], Pminus_data[4];
    float F_data[4], FT_data[4],temp_F_data[4];
    float B_data[4];
    float H_data[4], HT_data[4];
    float Q_data[4];
    float R_data[4];
    float K_data[4];
    float S_data[4], temp_matrix_data[4], temp_matrix_data1[4], temp_vector_data[4], temp_vector_data1[4];
} KalmanFilter_t;











#endif


