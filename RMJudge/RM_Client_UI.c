/*************************************************************
  ****************************(C) COPYRIGHT 2021 NCIST****************************
  * @file       UI_task.c/h
  * @brief      ��λ��ͨ�������Ӿ����ݴ���
  *
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V2.0.0     2021           zhuxunfu              finished
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 NCIST****************************
  */

#include "RM_Client_UI.h"
#include "protocol.h"
#include "IMUTASK.h"
#include "judgement_info.h"
#include "string.h"
#include "arm_math.h"
#include "Remote_Control.h"
#include "Variable.h"

#define Get_CRC16_Check_Sum_UI get_crc16_check_sum
#define Get_CRC8_Check_Sum_UI get_crc8_check_sum 
 
char key_flag = 0, UI_flag = 0;
extern ext_game_robot_state_t			  	GameRobotStat;				//0x0201
ext_robot_hurt_t hurt_data;
extern u32 Robot_ID,Cilent_ID;
float	x0_ui[2],y0_ui[2],x1_ui[2],y1_ui[2],superpower_x, super_ratio;
String_Data CH_FRICTION[2],CH_GIMBAL[2],CH_CHASSIC[2],Power[2],CH_BULLET[2],GDistance[2];
Graph_Data Shoot_Mode[2];


Graph_Data I1,I2;////I1����ǰ��·��Ԥ��		I2����ǰ��·��Ԥ��	
Graph_Data G8[2];//G8����������		G21, G22, G23, G24, G25, G26����׼��	
Graph_Data PC[2];
Graph_Data PC_S[2];
Graph_Data Cross[2];
char friction_off[4] = "SAFE",friction_on[4] = "SEMI",friction_run[5]="BURST",friction_auto[4]="AUTO";
char chassis_follow_mode[14]= "CHASSIC:FOLLOW",chassis_dodge_mode[14] = "CHASSIC: DODGE",chassic_sparate_mode[14]="CHASSIC: RELAX";
char track_normal[6] = "Normal",track_On[9] = "Excalibur";
char Magazine_off[12] = "MAGAZINE:OFF",Magazine_on[12] = "MAGAZINE: ON";
char Super_cap[5]={0};
char Distance[9]={"Mechanism"};
char OutPosts[9]={" OutPosts"};





void UI_task(void *pvParameters)
{	
	

	///�����һ��ID��UIȻ������һ���ṹ��ʹ�����IDȥˢ��UI Ŀǰ����ĸ�������ʹ��
	
	Char_Draw(&CH_FRICTION[0],"001",UI_Graph_ADD,1,UI_Color_Pink,18,4,2,1400,620,&friction_off[0]);//Ħ����״̬
	Char_Draw(&CH_CHASSIC[0],	"002",UI_Graph_ADD,1,UI_Color_Pink,18,13,2,180,700,&chassic_sparate_mode[0]);//����ģʽ
	Char_Draw(&CH_GIMBAL[0],	"003",UI_Graph_ADD,1,UI_Color_Pink,18,13,2,180,620,&track_normal[0]);//��̨״̬����
	Char_Draw(&CH_BULLET[0],	"004",UI_Graph_ADD,1,UI_Color_Pink,18,12,2,180,540,&Magazine_off[0]);//����״̬	//��ɫ����ڶ������ַ�����
	Char_Draw(&Power[0],			"005",UI_Graph_ADD,1,UI_Color_Cyan,40,5,2,1400,780,&Super_cap[0]);
	Char_Draw(&GDistance[0],  "007",UI_Graph_ADD,1,UI_Color_Pink,20,12,2,1400,700,&track_normal[0]);
	Circle_Draw(&PC[0],				"006",UI_Graph_ADD,0,UI_Color_Cyan,2,950.0f,500.0f,5.0f);		//��	120*40
	Line_Draw(&G8[0],         "008",UI_Graph_ADD,1,UI_Color_Green,2,960,500,960,540);	
	Line_Draw(&Shoot_Mode[0], "009",UI_Graph_ADD,1,UI_Color_Purplish_red,2,910,500,990,500);
	Line_Draw(&Cross[0], 			"010",UI_Graph_ADD,1,UI_Color_Purplish_red,2,950,800,950,200);
	Line_Draw(&Shoot_Mode[1], "011",UI_Graph_ADD,1,UI_Color_Purplish_red,2,950,490,960,490);
	Line_Draw(&Cross[1], 			"012",UI_Graph_ADD,1,UI_Color_Purplish_red,2,950,480,956,480);
	Rectangle_Draw(&PC_S[0],	"013",UI_Graph_ADD,1,UI_Color_Cyan,4,900,680,1020,400);
	Char_ReFresh(CH_FRICTION[0]);	
	Char_ReFresh(CH_CHASSIC[0]);
	Char_ReFresh(CH_BULLET[0]);
	Char_ReFresh(CH_GIMBAL[0]);
	Char_ReFresh(GDistance[0]);
	Char_ReFresh(Power[0]);

for(;;)
 {
	 
	 
	 if(FLAG.Key_State.V && key_flag == 0) //v��ʾ������ui
		{
			key_flag = 1;
			UI_flag = 0;
		}
		else if(!FLAG.Key_State.V)
		{
			UI_flag =1;
			key_flag = 0;
		}
		else UI_flag =1;
	 
	 
	if(UI_flag)
	{
		
/************************************************�����ַ�������*************************************************
**������*image Graph_Data���ͱ���ָ�룬���ڴ��ͼ������
        imagename[3]   ͼƬ���ƣ����ڱ�ʶ����
        Graph_Operate   ͼƬ��������ͷ�ļ�
        Graph_Layer    ͼ��0-9
        Graph_Color    ͼ����ɫ
        Graph_Width    ͼ���߿�
        Graph_Size     �ֺ�
        Graph_Digit    �ַ�����
        Start_x��Start_x    ��ʼ����
        *Char_Data          �������ַ�����ʼ��ַ
**********************************************************************************************************/
				

	//Char_Draw(&CH_CHASSIC[5],"078",UI_Graph_ADD,1,UI_Color_Pink,18,13,2,180,620,&track_normal[0]);//��̨״̬����
			if(!FLAG.Key_State.E)
			{
				memset(&CH_FRICTION[1],0,sizeof(CH_FRICTION[1]));
				Char_Draw(&CH_FRICTION[1],"001",UI_Graph_Change,1,UI_Color_Green,18,4,2,1400,620,&friction_off[0]);//Ħ����״̬
				
			}
			else
			{
				if(Cutting==0)
				{
					memset(&CH_FRICTION[1],0,sizeof(CH_FRICTION[1]));
					Char_Draw(&CH_FRICTION[1],"001",UI_Graph_Change,1,UI_Color_Purplish_red,18,4,2,1400,620,&friction_on[0]);//Ħ����״̬	
				}
				else if(Cutting==1)
				{
					memset(&CH_FRICTION[1],0,sizeof(CH_FRICTION[1]));
					Char_Draw(&CH_FRICTION[1],"001",UI_Graph_Change,1,UI_Color_Purplish_red,18,5,2,1400,620,&friction_run[0]);//Ħ����״̬	
				}
				else if(Cutting==2)
				{
					memset(&CH_FRICTION[1],0,sizeof(CH_FRICTION[1]));
					Char_Draw(&CH_FRICTION[1],"001",UI_Graph_Change,1,UI_Color_Purplish_red,18,4,2,1400,620,&friction_auto[0]);//Ħ����״̬	
				
				}
				
			}
			
			
			if(FLAG.Key_State.R)
			{
				memset(&CH_BULLET[1],0,sizeof(CH_BULLET[1]));
				Char_Draw(&CH_BULLET[1],	"004",UI_Graph_Change,1,UI_Color_Green,18,12,2,180,540,&Magazine_on[0]);//����״̬	//��ɫ����ڶ������ַ�����
				
			}
			else
			{
				memset(&CH_BULLET[1],0,sizeof(CH_BULLET[1]));
				Char_Draw(&CH_BULLET[1],	"004",UI_Graph_Change,1,UI_Color_Pink,18,12,2,180,540,&Magazine_off[0]);//����״̬	//��ɫ����ڶ������ַ�����
			
			}
			
			
			if(FLAG.Key_State.B)
			{
				memset(&CH_GIMBAL[1],0,sizeof(CH_GIMBAL[1]));
				Char_Draw(&CH_GIMBAL[1],	"003",UI_Graph_Change,1,UI_Color_Yellow,18,9,2,180,620,&track_On[0]);//��̨״̬����
			}
			else
			{
				memset(&CH_GIMBAL[1],0,sizeof(CH_GIMBAL[1]));
				Char_Draw(&CH_GIMBAL[1],	"003",UI_Graph_Change,1,UI_Color_Pink,18,6,2,180,620,&track_normal[0]);//��̨״̬����
			}
			
				
		
			if(RC_Mode==CHASSIS_DODGE_MODE)
			{
				memset(&CH_CHASSIC[1],0,sizeof(CH_CHASSIC[1]));
				Char_Draw(&CH_CHASSIC[1],"002",UI_Graph_Change,1,UI_Color_Green,18,14,2,180,700,&chassis_dodge_mode[0]);
			}
			else if(RC_Mode==CHASSIS_RELAX)
			{
				memset(&CH_CHASSIC[1],0,sizeof(CH_CHASSIC[1]));
				Char_Draw(&CH_CHASSIC[1],"002",UI_Graph_Change,1,UI_Color_Yellow,18,14,2,180,700,&chassic_sparate_mode[0]);
			}
			else if(RC_Mode==CHASSIS_FOLLOW_GIMBAL)
			{
				memset(&CH_CHASSIC[1],0,sizeof(CH_CHASSIC[1]));
				Char_Draw(&CH_CHASSIC[1],"002",UI_Graph_Change,1,UI_Color_Yellow,18,14,2,180,700,&chassis_follow_mode[0]);
			}
		
			
			
			
/****************************************����***************************************************/			
			
	
			
/****************************************��������***************************************************/

		superpower_x = Super_power.volt;

		if(superpower_x >= 22000.0f)
		{
				superpower_x = 22000.0f;
		}
		else if(superpower_x <= 12000.0f)
		{
				superpower_x = 12000.0f;
		}
		super_ratio = (superpower_x - 12000.0f) / 10000.0f * 100.0f;
					
		
		Line_Draw(&G8[1],"008",UI_Graph_Change,1,UI_Color_Green,2,x0_ui[0],y0_ui[0],x1_ui[0],y1_ui[0]);
		
		
		
		sprintf(Super_cap, "%0.3f", super_ratio);
		memset(&Power[1],0,sizeof(Power[1]));
		Char_Draw(&Power[1],"005",UI_Graph_Change,1,UI_Color_Cyan,40,5,2,1400,780,&Super_cap[0]);
			
		
		if(FLAG.Key_State.G==1)
		{
			memset(&GDistance[1],0,sizeof(GDistance[1]));
			Char_Draw(&GDistance[1],   "007",UI_Graph_Change,1,UI_Color_Cyan,20,9,2,1400,720,&OutPosts[0]);
		}
		else if(FLAG.Key_State.G==2)
		{
			memset(&GDistance[1],0,sizeof(GDistance[1]));
			Char_Draw(&GDistance[1],   "007",UI_Graph_Change,1,UI_Color_Cyan,20,6,2,1400,720,&Distance[0]);
		}
		else
		{
			memset(&GDistance[1],0,sizeof(GDistance[1]));
			Char_Draw(&GDistance[1],   "007",UI_Graph_Change,1,UI_Color_Cyan,20,6,2,1400,720,&track_normal[0]);
		
		}
/****************************************����λ��***************************************************/
		
	
		
		if(GameRobotStat.robot_id == 3)
			{
				Robot_ID = 3;
				Cilent_ID = 0x0103;
			}

			if(GameRobotStat.robot_id == 4)
			{
				Robot_ID = 4;
				Cilent_ID = 0x0104;
			}

			if(GameRobotStat.robot_id == 5)
			{
				Robot_ID = 5;
				Cilent_ID = 0x0105;
			}

			if(GameRobotStat.robot_id == 103)
			{
				Robot_ID = 103;
				Cilent_ID = 0x0167;
			}

			if(GameRobotStat.robot_id == 104)
			{
				Robot_ID = 104;
				Cilent_ID = 0x0168;
			}

			if(GameRobotStat.robot_id == 105)
			{
				Robot_ID = 105;
				Cilent_ID = 0x0169;
			}
			
		Char_ReFresh(GDistance[0]);
		Char_ReFresh(CH_FRICTION[0]);	
		Char_ReFresh(CH_CHASSIC[0]);
		Char_ReFresh(CH_BULLET[0]);
		Char_ReFresh(CH_GIMBAL[0]);
		Char_ReFresh(Power[0]);
		
		
		Char_ReFresh(GDistance[1]);
		Char_ReFresh(CH_FRICTION[1]);	
		Char_ReFresh(CH_CHASSIC[1]);
		Char_ReFresh(CH_BULLET[1]);
		Char_ReFresh(CH_GIMBAL[1]);
		Char_ReFresh(Power[1]);
		
						
		UI_ReFresh(2,G8[0],G8[1]);
		UI_ReFresh(2,Shoot_Mode[0],Shoot_Mode[1]);
		UI_ReFresh(2,PC[0],PC[1]);
		UI_ReFresh(2,PC_S[0],PC_S[1]);
		UI_ReFresh(2,Cross[0],Cross[1]);
	}
		vTaskDelay(1);
	}
}














/****************************************************************************************/
unsigned char UI_Seq;                      //�����
u32 Robot_ID,Cilent_ID;
/****************************************��������ӳ��************************************/
void UI_SendByte(unsigned char ch)
{
   USART_SendData(USART1,ch);
   while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	
}

/********************************************ɾ������*************************************
**������Del_Operate  ��Ӧͷ�ļ�ɾ������
        Del_Layer    Ҫɾ���Ĳ� ȡֵ0-9
*****************************************************************************************/

void UI_Delete(u8 Del_Operate,u8 Del_Layer)
{

   unsigned char *framepoint;                      //��дָ��
   u16 frametail=0xFFFF;                        //CRC16У��ֵ
   int loop_control;                       //For����ѭ������
   
   UI_Packhead framehead;
   UI_Data_Operate datahead;
   UI_Data_Delete del;
   
   framepoint=(unsigned char *)&framehead;
   
   framehead.SOF=UI_SOF;
   framehead.Data_Length=8;
   framehead.Seq=UI_Seq;
   framehead.CRC8=Get_CRC8_Check_Sum_UI(framepoint,4,0xFF);
   framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //����ͷ����
   
   datahead.Data_ID=UI_Data_ID_Del;
   datahead.Sender_ID=Robot_ID;
   datahead.Receiver_ID=Cilent_ID;                          //����������
   
   del.Delete_Operate=Del_Operate;
   del.Layer=Del_Layer;                                     //������Ϣ
   
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(framehead),frametail);
   framepoint=(unsigned char *)&datahead;
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(datahead),frametail);
   framepoint=(unsigned char *)&del;
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(del),frametail);  //CRC16У��ֵ����
   
   framepoint=(unsigned char *)&framehead;
   for(loop_control=0;loop_control<sizeof(framehead);loop_control++)
   {
      UI_SendByte(*framepoint);
      framepoint++;
   }
   framepoint=(unsigned char *)&datahead;
   for(loop_control=0;loop_control<sizeof(datahead);loop_control++)
   {
      UI_SendByte(*framepoint);
      framepoint++;
   }
   framepoint=(unsigned char *)&del;
   for(loop_control=0;loop_control<sizeof(del);loop_control++)
   {
      UI_SendByte(*framepoint);
      framepoint++;
   }                                                                 //��������֡
   framepoint=(unsigned char *)&frametail;
   for(loop_control=0;loop_control<sizeof(frametail);loop_control++)
   {
      UI_SendByte(*framepoint);
      framepoint++;                                                  //����CRC16У��ֵ
   }
   
   //UI_Seq++;                                                         //�����+1
}
/************************************************����ֱ��*************************************************
**������*image Graph_Data���ͱ���ָ�룬���ڴ��ͼ������
        imagename[3]   ͼƬ���ƣ����ڱ�ʶ����
        Graph_Operate   ͼƬ��������ͷ�ļ�
        Graph_Layer    ͼ��0-9
        Graph_Color    ͼ����ɫ
        Graph_Width    ͼ���߿�
        Start_x��Start_x    ��ʼ����
        End_x��End_y   ��������
**********************************************************************************************************/
        
void Line_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
   int i;
   for(i=0;i<3&&imagename[i]!=0;i++)
   image->graphic_name[i]=imagename[i];
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/************************************************���ƾ���*************************************************
**������*image Graph_Data���ͱ���ָ�룬���ڴ��ͼ������
        imagename[3]   ͼƬ���ƣ����ڱ�ʶ����
        Graph_Operate   ͼƬ��������ͷ�ļ�
        Graph_Layer    ͼ��0-9
        Graph_Color    ͼ����ɫ
        Graph_Width    ͼ���߿�
        Start_x��Start_x    ��ʼ����
        End_x��End_y   �������꣨�Զ������꣩
**********************************************************************************************************/
        
void Rectangle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 End_x,u32 End_y)
{
   int i;
   for(i=0;i<3&&imagename[i]!=0;i++)
      image->graphic_name[2-i]=imagename[i];
   image->graphic_tpye = UI_Graph_Rectangle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->end_x = End_x;
   image->end_y = End_y;
}

/************************************************������Բ*************************************************
**������*image Graph_Data���ͱ���ָ�룬���ڴ��ͼ������
        imagename[3]   ͼƬ���ƣ����ڱ�ʶ����
        Graph_Operate   ͼƬ��������ͷ�ļ�
        Graph_Layer    ͼ��0-9
        Graph_Color    ͼ����ɫ
        Graph_Width    ͼ���߿�
        Start_x��Start_x    Բ������
        Graph_Radius  ͼ�ΰ뾶
**********************************************************************************************************/
        
void Circle_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 Graph_Radius)
{
   int i;
   for(i=0;i<3&&imagename[i]!=0;i++)
      image->graphic_name[i]=imagename[i];
   image->graphic_tpye = UI_Graph_Circle;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->radius = Graph_Radius;
}

/************************************************����Բ��*************************************************
**������*image Graph_Data���ͱ���ָ�룬���ڴ��ͼ������
        imagename[3]   ͼƬ���ƣ����ڱ�ʶ����
        Graph_Operate   ͼƬ��������ͷ�ļ�
        Graph_Layer    ͼ��0-9
        Graph_Color    ͼ����ɫ
        Graph_Width    ͼ���߿�
        Graph_StartAngle,Graph_EndAngle    ��ʼ����ֹ�Ƕ�
        Start_y,Start_y    Բ������
        x_Length,y_Length   x,y�������᳤���ο���Բ
**********************************************************************************************************/
        
void Arc_Draw(Graph_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_StartAngle,u32 Graph_EndAngle,u32 Graph_Width,u32 Start_x,u32 Start_y,u32 x_Length,u32 y_Length)
{
   int i;
   
   for(i=0;i<3&&imagename[i]!=0;i++)
      image->graphic_name[i]=imagename[i];
   image->graphic_tpye = UI_Graph_Arc;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->start_angle = Graph_StartAngle;
   image->end_angle = Graph_EndAngle;
   image->end_x = x_Length;
   image->end_y = y_Length;
}



/************************************************���Ƹ���������*************************************************
**������*image Graph_Data���ͱ���ָ�룬���ڴ��ͼ������
        imagename[3]   ͼƬ���ƣ����ڱ�ʶ����
        Graph_Operate   ͼƬ��������ͷ�ļ�
        Graph_Layer    ͼ��0-9
        Graph_Color    ͼ����ɫ
        Graph_Width    ͼ���߿�
        Graph_Size     �ֺ�
        Graph_Digit    С��λ��
        Start_x��Start_x    ��ʼ����
        Graph_Float   Ҫ��ʾ�ı���
**********************************************************************************************************/
        
void Float_Draw(Float_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,float Graph_Float)
{
   int i;
   
   for(i=0;i<3&&imagename[i]!=0;i++)
      image->graphic_name[2-i]=imagename[i];
   image->graphic_tpye = UI_Graph_Float;
   image->operate_tpye = Graph_Operate;
   image->layer = Graph_Layer;
   image->color = Graph_Color;
   image->width = Graph_Width;
   image->start_x = Start_x;
   image->start_y = Start_y;
   image->start_angle = Graph_Size;
   image->end_angle = Graph_Digit;
   image->graph_Float = Graph_Float;
}



/************************************************�����ַ�������*************************************************
**������*image Graph_Data���ͱ���ָ�룬���ڴ��ͼ������
        imagename[3]   ͼƬ���ƣ����ڱ�ʶ����
        Graph_Operate   ͼƬ��������ͷ�ļ�
        Graph_Layer    ͼ��0-9
        Graph_Color    ͼ����ɫ
        Graph_Width    ͼ���߿�
        Graph_Size     �ֺ�
        Graph_Digit    �ַ�����
        Start_x��Start_x    ��ʼ����
        *Char_Data          �������ַ�����ʼ��ַ
**********************************************************************************************************/
        
void Char_Draw(String_Data *image,char imagename[3],u32 Graph_Operate,u32 Graph_Layer,u32 Graph_Color,u32 Graph_Size,u32 Graph_Digit,u32 Graph_Width,u32 Start_x,u32 Start_y,char *Char_Data)
{
   int i;
   
   for(i=0;i<3&&imagename[i]!=0;i++)
      image->Graph_Control.graphic_name[i]=imagename[i];
   image->Graph_Control.graphic_tpye = UI_Graph_Char;
   image->Graph_Control.operate_tpye = Graph_Operate;
   image->Graph_Control.layer = Graph_Layer;
   image->Graph_Control.color = Graph_Color;
   image->Graph_Control.width = Graph_Width;
   image->Graph_Control.start_x = Start_x;
   image->Graph_Control.start_y = Start_y;
   image->Graph_Control.start_angle = Graph_Size;
   image->Graph_Control.end_angle = Graph_Digit;
   
   for(i=0;i<Graph_Digit;i++)
   {
      image->show_Data[i]=*Char_Data;
      Char_Data++;
   }
}

/************************************************UI���ͺ�����ʹ������Ч��*********************************
**������ cnt   ͼ�θ���
         ...   ͼ�α�������


Tips�����ú���ֻ������1��2��5��7��ͼ�Σ�������ĿЭ��δ�漰
**********************************************************************************************************/
int UI_ReFresh(int cnt,...)
{
   int i,n;
   Graph_Data imageData;
   unsigned char *framepoint;                      //��дָ��
   u16 frametail=0xFFFF;                        //CRC16У��ֵ
   
   UI_Packhead framehead;
   UI_Data_Operate datahead;
   
   va_list ap;
   va_start(ap,cnt);
   
   framepoint=(unsigned char *)&framehead;
   framehead.SOF=UI_SOF;
   framehead.Data_Length=6+cnt*15;
   framehead.Seq=UI_Seq;
   framehead.CRC8=Get_CRC8_Check_Sum_UI(framepoint,4,0xFF);
   framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //����ͷ����
   
   switch(cnt)
   {
      case 1:
         datahead.Data_ID=UI_Data_ID_Draw1;
         break;
      case 2:
         datahead.Data_ID=UI_Data_ID_Draw2;
         break;
      case 5:
         datahead.Data_ID=UI_Data_ID_Draw5;
         break;
      case 7:
         datahead.Data_ID=UI_Data_ID_Draw7;
         break;
      default:
         return (-1);
   }
   datahead.Sender_ID=Robot_ID;
   datahead.Receiver_ID=Cilent_ID;                          //����������
   
   framepoint=(unsigned char *)&framehead;
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(framehead),frametail);
   framepoint=(unsigned char *)&datahead;
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(datahead),frametail);          //CRC16У��ֵ���㣨���֣�
   
   framepoint=(unsigned char *)&framehead;
   for(i=0;i<sizeof(framehead);i++)
   {
      UI_SendByte(*framepoint);
      framepoint++;
   }
   framepoint=(unsigned char *)&datahead;
   for(i=0;i<sizeof(datahead);i++)
   {
      UI_SendByte(*framepoint);
      framepoint++;
   }
   
   for(i=0;i<cnt;i++)
   {
      imageData=va_arg(ap,Graph_Data);
      
      framepoint=(unsigned char *)&imageData;
      frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(imageData),frametail);             //CRC16У��
      
      for(n=0;n<sizeof(imageData);n++)
      {
         UI_SendByte(*framepoint);
         framepoint++;             
      }                                               //����ͼƬ֡
   }
   framepoint=(unsigned char *)&frametail;
   for(i=0;i<sizeof(frametail);i++)
   {
      UI_SendByte(*framepoint);
      framepoint++;                                                  //����CRC16У��ֵ
   }
   
   va_end(ap);
   
   //UI_Seq++;                                                         //�����+1
   return 0;
}


/************************************************UI�����ַ���ʹ������Ч��*********************************
**������ cnt   ͼ�θ���
         ...   ͼ�α�������


Tips�����ú���ֻ������1��2��5��7��ͼ�Σ�������ĿЭ��δ�漰
**********************************************************************************************************/
int Char_ReFresh(String_Data string_Data)
{
   int i;
   String_Data imageData;
   unsigned char *framepoint;                      //��дָ��
   u16 frametail=0xFFFF;                        //CRC16У��ֵ
   
   UI_Packhead framehead;
   UI_Data_Operate datahead;
   imageData=string_Data;
   
   
   framepoint=(unsigned char *)&framehead;
   framehead.SOF=UI_SOF;
   framehead.Data_Length=6+45;
   framehead.Seq=UI_Seq;
   framehead.CRC8=Get_CRC8_Check_Sum_UI(framepoint,4,0xFF);
   framehead.CMD_ID=UI_CMD_Robo_Exchange;                   //����ͷ����
   

   datahead.Data_ID=UI_Data_ID_DrawChar;

   datahead.Sender_ID=Robot_ID;
   datahead.Receiver_ID=Cilent_ID;                          //����������
   
   framepoint=(unsigned char *)&framehead;
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(framehead),frametail);
   framepoint=(unsigned char *)&datahead;
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(datahead),frametail);
   framepoint=(unsigned char *)&imageData;
   frametail=Get_CRC16_Check_Sum_UI(framepoint,sizeof(imageData),frametail);             //CRC16У��   //CRC16У��ֵ���㣨���֣�
   
   framepoint=(unsigned char *)&framehead;
   for(i=0;i<sizeof(framehead);i++)
   {
      UI_SendByte(*framepoint);
      framepoint++;
   }
   framepoint=(unsigned char *)&datahead;
   for(i=0;i<sizeof(datahead);i++)
   {
      UI_SendByte(*framepoint);
      framepoint++;
   }                                                   //���Ͳ�������  
   framepoint=(unsigned char *)&imageData;
   for(i=0;i<sizeof(imageData);i++)
   {
      UI_SendByte(*framepoint);
      framepoint++;             
   }                                               //����ͼƬ֡
   
   
   
   framepoint=(unsigned char *)&frametail;
   for(i=0;i<sizeof(frametail);i++)
   {
      UI_SendByte(*framepoint);
      framepoint++;                                                  //����CRC16У��ֵ
   }
   
   
   //UI_Seq++;                                                         //�����+1
   return 0;
}



