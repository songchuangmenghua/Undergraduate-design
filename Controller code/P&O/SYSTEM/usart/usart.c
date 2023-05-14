#include "usart.h"
#include "LED.h"
#include "irr.h"
#include "hil.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/6/23
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART1_Handler; //UART���

// ��������
u8 USART3_RX_BUF[USART3_REC_LEN]; 
u8 USART3_RX_STA = 0;
u8 USART3_RxBuffer[USART3_RXBUFFERSIZE];
UART_HandleTypeDef USART3_Handler; //UART���
IRRtypeDef IRR;

//��������
u8 USART2_RX_BUF[USART2_REC_LEN]; 
u8 USART2_RX_STA = 0;
u8 USART2_RxBuffer[USART2_RXBUFFERSIZE];
UART_HandleTypeDef USART2_Handler;         //UART���
HILtypeDef HIL;


//COM3���ڽ��շ�����������
 void AV87110_USART3_init(u32 bound)  
{
	USART3_Handler.Instance=USART3;					              // USART3
	USART3_Handler.Init.BaudRate=bound;				            // ������ 9600
	USART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;    // �ֳ�Ϊ8λ���ݸ�ʽ
	USART3_Handler.Init.StopBits=UART_STOPBITS_1;	        // һ��ֹͣλ
	USART3_Handler.Init.Parity=UART_PARITY_NONE;          // UART_PARITY_EVEN;		//żУ��
	USART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;    // ��Ӳ������
	USART3_Handler.Init.Mode=UART_MODE_TX_RX;		          // �շ�ģʽ
	HAL_UART_Init(&USART3_Handler);					              // HAL_UART_Init()��ʹ��USART3
	
	HAL_UART_Receive_IT(&USART3_Handler, (u8 *)USART3_RxBuffer, USART3_RXBUFFERSIZE);
}

//COM2
void METV53USART2_init(u32 bound)
{
	USART2_Handler.Instance=USART2;					              // USART2
	USART2_Handler.Init.BaudRate=bound;				            // ������
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;    // �ֳ�Ϊ8λ���ݸ�ʽ
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;	        // һ��ֹͣλ
	USART2_Handler.Init.Parity=UART_PARITY_NONE;       		//��У��
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;    // ��Ӳ������
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;		          // �շ�ģʽ
	HAL_UART_Init(&USART2_Handler);					              // HAL_UART_Init()��ʹ��USART2
	
	HAL_UART_Receive_IT(&USART2_Handler, (u8 *)USART2_RxBuffer, USART2_RXBUFFERSIZE);
}


void uart_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}


//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
#if EN_USART1_RX
		//__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}	
	
	if(huart->Instance==USART3)               //����Ǵ���3�����д���3 MSP��ʼ��
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			      //ʹ��GPIOBʱ��
		__HAL_RCC_USART3_CLK_ENABLE();			    //ʹ��USART3ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_10;			      //PB10 = TX
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		  //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			    //����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		  //����
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART
		HAL_GPIO_Init(GPIOB, &GPIO_Initure);	   //��ʼ����ѡ��GPIO����ΪA��

		GPIO_Initure.Pin=GPIO_PIN_11;			       //PB11 = RX
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	 //��ʼ��PB11
		
#if EN_USART3_RX	
        //__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�		
		HAL_NVIC_EnableIRQ(USART3_IRQn);		//ʹ��USART2�ж�ͨ��
		HAL_NVIC_SetPriority(USART3_IRQn,2,1);	//��ռ���ȼ�3�������ȼ�3
#endif			
	}
		if(huart->Instance==USART2)               //����Ǵ���2�����д���2 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			      //ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			    //ʹ��USART2ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;			      //PA2 = TX
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		  //�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			    //����
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;		  //����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   //��ʼ����ѡ��GPIO����ΪA��
		
		
#if EN_USART2_RX	
        //__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�		
		HAL_NVIC_EnableIRQ(USART2_IRQn);		//ʹ��USART2�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,2,1);	//��ռ���ȼ�3�������ȼ�3
#endif		
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	u8 rec;
	//u8 i;
	
	if(huart->Instance==USART1)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
	}
	
	if(huart->Instance==USART3) //����Ǵ���3
	{
		rec = *(--(huart->pRxBuffPtr));
		//printf("rec = %02X ", rec);
//		LED0_Toggle;                    // ���յ�������˸LED0
		USART3_RX_BUF[USART3_RX_STA] = rec;
		USART3_RX_STA++;	
		
		if((USART3_RX_STA&0x001F)>(USART3_REC_LEN-1))
		{
			 IRR = AV87110_RS232_Decode(USART3_RX_BUF);
//			for(i=0; i<USART3_RX_STA; i++){
//			// printf("USART3_RX_STA = %d \n",USART3_RX_STA);
//			 printf("%02X ", USART3_RX_BUF[i]);
//			}
//			printf(" \n");
			 USART3_RX_STA = 0; 
		} 
	}	
		if(huart->Instance==USART2) //����Ǵ���2
	{
		rec = *(--(huart->pRxBuffPtr));
		//printf("rec = %02X ", rec);
//		LED0_Toggle;                    // ���յ�������˸LED0
		USART2_RX_BUF[USART2_RX_STA] = rec;
		USART2_RX_STA++;	
		
		if((USART2_RX_STA&0xFF)>(USART2_REC_LEN-1))
		{
			 HIL = METV53_Decode(USART2_RX_BUF);
//			for(i=0; i<USART3_RX_STA; i++){
//			// printf("USART3_RX_STA = %d \n",USART3_RX_STA);
//			 printf("%02X ", USART3_RX_BUF[i]);
//			}
//			printf(" \n");
			 USART2_RX_STA = 0; 
		} 
//		printf("a");
	}	
}	
	

//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;

	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}

} 


void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
  u32 maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&USART3_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&USART3_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&USART3_Handler,(u8 *)USART3_RxBuffer, USART3_RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}

} 


void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
  u32 maxDelay=0x1FFFF;

	HAL_UART_IRQHandler(&USART2_Handler);	//����HAL���жϴ����ú���
	
	timeout=0;
    while (HAL_UART_GetState(&USART2_Handler)!=HAL_UART_STATE_READY)//�ȴ�����
	{
        timeout++;////��ʱ����
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&USART2_Handler,(u8 *)USART2_RxBuffer, USART2_RXBUFFERSIZE)!=HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
        timeout++; //��ʱ����
        if(timeout>maxDelay) break;	
	}
//	printf("b");

} 








 

 




