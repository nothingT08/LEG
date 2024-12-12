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
	//GPIO端口设置
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

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate 						= 		bound;														
	USART_InitStructure.USART_WordLength 					= 		USART_WordLength_8b;							
	USART_InitStructure.USART_StopBits 						= 		USART_StopBits_1;									
	USART_InitStructure.USART_Parity 							= 		USART_Parity_No;									
	USART_InitStructure.USART_HardwareFlowControl = 		USART_HardwareFlowControl_None;		
	USART_InitStructure.USART_Mode 								= 		USART_Mode_Rx | USART_Mode_Tx;		
	USART_Init(USART1, &USART_InitStructure); 
	
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;				//抢占优先级4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;						//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	


	
	TxCOM1 = xQueueCreate(50, sizeof(u8));
	if( TxCOM1 == 0 )
		{
			/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
		}
	
		USART_Cmd(USART1, ENABLE);  //使能串口1 
}






void USART1_IRQHandler(void)                	
{
	
	BaseType_t  TaskWoken;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{

			USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
			(void)USART1->SR;
			(void)USART1->DR;
			Res[0] =USART_ReceiveData(USART1);				//读取接收到的数据
	
			xQueueSendFromISR(TxCOM1,&Res,&TaskWoken);
	} 
} 









