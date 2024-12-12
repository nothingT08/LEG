#include "TaskInit.h"  	
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"




u16 USART_RX_STA=0;   


int n=0;
unsigned char Res[11];

QueueHandle_t TxCOM1;


void Device_Usart1_ENABLE_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;																			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 																				
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 																					
	GPIO_Init(GPIOA,&GPIO_InitStructure); 																								

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate 						= 		bound;														
	USART_InitStructure.USART_WordLength 					= 		USART_WordLength_8b;							
	USART_InitStructure.USART_StopBits 						= 		USART_StopBits_1;									
	USART_InitStructure.USART_Parity 							= 		USART_Parity_No;									
	USART_InitStructure.USART_HardwareFlowControl = 		USART_HardwareFlowControl_None;		
	USART_InitStructure.USART_Mode 								= 		USART_Mode_Rx | USART_Mode_Tx;		
	USART_Init(USART1, &USART_InitStructure); 
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;				//��ռ���ȼ�4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;						//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	


	
	TxCOM1 = xQueueCreate(50, sizeof(u8));
	if( TxCOM1 == 0 )
		{
			/* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
		}
	
		USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
}






void USART1_IRQHandler(void)                	
{
	
	BaseType_t  TaskWoken;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{

			USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
			(void)USART1->SR;
			(void)USART1->DR;
			Res[0] =USART_ReceiveData(USART1);				//��ȡ���յ�������
	
			xQueueSendFromISR(TxCOM1,&Res,&TaskWoken);
	} 
} 









