#include "LASER.h"
#include "sys.h"
#include "delay.h"
#include "string.h"
#include "stm32f4xx.h"
void UASRT_Init(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
	 /* -------------- Enable Module Clock Source ----------------------------*/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5); //PC11  usart4 rx

		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO_Speed_100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOD, &GPIO_InitStructure);


		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_Mode = USART_Mode_Rx;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(UART5, &USART_InitStructure);
		USART_Cmd(UART5, ENABLE);
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
		
		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7;				//抢占优先级4
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;						//子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
		NVIC_Init(&NVIC_InitStructure);	


		
			

}

uint8_t LASER_DISTAN_BUFF[50];
uint8_t LASER_DISTAN_Copy[50];
uint8_t LEN=0;
uint16_t Laser_DIS=0;
uint16_t SUM_C;
uint16_t SUM_C_F;
void UART5_IRQHandler(void)                	
{
	uint8_t Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  
		{

			USART_ClearITPendingBit(UART5, USART_IT_RXNE);	
			(void)UART5->SR;
			(void)UART5->DR;
			Res =USART_ReceiveData(UART5);
			LASER_DISTAN_BUFF[LEN]=Res;
			LEN++;
			if(LASER_DISTAN_BUFF[0]!=0x5A)
			{
				memcpy(LASER_DISTAN_BUFF,LASER_DISTAN_Copy,8);
				LEN=0;
			}
			else
			{
//				if(LASER_DISTAN_BUFF[1]!=0x5A)
//				{
//					memcpy(LASER_DISTAN_BUFF,LASER_DISTAN_Copy,8);
//					LEN=0;
//				}
//				else
				{
					if(LEN%8==0)
					{
						for(int i=0;i<7;i++)
						{	
							SUM_C+=LASER_DISTAN_BUFF[i]&0x00ff;
						}
						if(SUM_C==LASER_DISTAN_BUFF[7])
						{
							Laser_DIS=LASER_DISTAN_BUFF[4]<<8|LASER_DISTAN_BUFF[5];
						}
					}
				}
			}
			LEN%=8;
			SUM_C=0;
		} 
} 
