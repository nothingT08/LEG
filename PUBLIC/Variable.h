#ifndef __Variable_H_
#define __Variable_H_

#include "sys.h"
#include "stm32f4xx.h" 
#include "arm_math.h"


//���ļ����ڸ��������ͳһ��ʼ���Լ����ֺ궨���д�룬����״̬�ڸ��ļ����������������ͳһ�ڴ���������Ϊʱ��������ļ��д��ڱ���δд����ļ�





//����������ת�Ƕ�xitaΪ��
#define  pai 											 3.1415926f

#define  RadtoTheat 							 180.f/pai
#define  TheattoRad  							 pai/180.f
#define  CAN_3508_ALL_ID					 0x200
#define  CAN_6020_ALL_ID 					 0x1FF
#define  WARNING_REMAIN_POWER 		 60

#define  CHASSIS_RELAX           	  0//��������ģʽ
#define  CHASSIS_INIT           	  1
#define  CHASSIS_FOLLOW_GIMBAL  	  2//�Զ�������̨
#define  CHASSIS_SEPARATE_GIMBAL    3//��̨����
#define  CHASSIS_DODGE_MODE         4//С����
#define  CHASSIS_LEG_INIT_MODE      5//�Ȳ���ʼ��


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
    uint8_t id     :4;      // ���ID: 0,1...,13,14 15��ʾ�����е���㲥����(��ʱ�޷���)
    uint8_t status :3;      // ����ģʽ: 0.���� 1.FOC�ջ� 2.������У׼ 3.����
    uint8_t none   :1;      // ����λ
} SIS_Mode_t;   // ����ģʽ 1Byte

typedef struct
{
    uint8_t id     :4;      // ���ID: 0,1...,13,14 15��ʾ�����е���㲥����(��ʱ�޷���)
    uint8_t status :3;      // ����ģʽ: 0.���� 1.FOC�ջ� 2.������У׼ 3.����
    uint8_t none   :1;      // ����λ
} RIS_Mode_t;   // ����ģʽ 1Byte


typedef struct
{
    int16_t tor_des;        // �����ؽ����Ť�� unit: N.m      (q8)
    int16_t spd_des;        // �����ؽ�����ٶ� unit: rad/s    (q8)
    int32_t pos_des;        // �����ؽ����λ�� unit: rad      (q15)
    int16_t k_pos;          // �����ؽڸն�ϵ�� unit: -1.0-1.0 (q15)
    int16_t k_spd;          // �����ؽ�����ϵ�� unit: -1.0-1.0 (q15)
    
} SIS_Comd_t;   // ���Ʋ��� 12Byte


typedef struct
{
	int16_t MERR:3;
	int16_t Force:12;
	uint16_t Re:1;

}ERR;

typedef struct
{
    int16_t  tor;        // �����ؽ����Ť�� unit: N.m      (q8)
    int16_t  Speed;        // �����ؽ�����ٶ� unit: rad/s    (q8)
    int32_t  Pos;        // �����ؽ����λ�� unit: rad      (q15)
    int8_t 	 Temp;          // �����ؽڸն�ϵ�� unit: -1.0-1.0 (q15)
    ERR      ERRFOR;          // �����ؽ�����ϵ�� unit: -1.0-1.0 (q15)
    
} RIS_Comd_t;   // ���Ʋ��� 12Byte




typedef struct
{
    // ���� ��������������ݰ�
    uint8_t head[2];    // ��ͷ         2Byte
    SIS_Mode_t mode;    // �������ģʽ  1Byte
    SIS_Comd_t comd;    // ����������� 12Byte
    uint16_t   CRC16;   // CRC          2Byte
} ControlData_t;     //��������������ݰ�

typedef struct
{
    // ���� ��������������ݰ�
    uint8_t head[2];    // ��ͷ         2Byte
    RIS_Mode_t mode;    // �������ģʽ  1Byte
    RIS_Comd_t comd;    // ����������� 12Byte
    uint16_t   CRC16;   // CRC          2Byte
} ReceData_t;     //��������������ݰ�

typedef union 
{
	ControlData_t motor_send_data;  
	uint8_t Data[17];

}Send;

#pragma pack()

typedef struct
{
	uint8_t ERR;								//����¶�
	float Encoder;									//���ת��
	float SPEED;								//������λ��
	float Tor;											//ת�ص���
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
	uint8_t Motor_Temp;								//����¶�
	uint8_t SPEED[2];									//���ת��
	uint8_t Encoder[2];								//������λ��
	uint8_t Iq[2];											//ת�ص���
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

    uint8_t *MeasurementMap;      // ������״̬�Ĺ�ϵ how measurement relates to the state
    float *MeasurementDegree;     // ����ֵ��ӦH����Ԫ��ֵ elements of each measurement in H
    float *MatR_DiagonalElements; // ���ⷽ�� variance for each measurement
    float *StateMinVariance;      // ��С���� ���ⷽ���������               suppress filter excessive convergence
    uint8_t *temp;

    // ����û����庯��ʹ��,��Ϊ��־λ�����ж��Ƿ�Ҫ������׼KF����������е�����һ��
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

    // �û����庯��,�����滻����չ��׼KF�Ĺ���
    void (*User_Func0_f)(struct kf_t *kf);
    void (*User_Func1_f)(struct kf_t *kf);
    void (*User_Func2_f)(struct kf_t *kf);
    void (*User_Func3_f)(struct kf_t *kf);
    void (*User_Func4_f)(struct kf_t *kf);
    void (*User_Func5_f)(struct kf_t *kf);
    void (*User_Func6_f)(struct kf_t *kf);
    
    // ����洢�ռ�ָ��
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


