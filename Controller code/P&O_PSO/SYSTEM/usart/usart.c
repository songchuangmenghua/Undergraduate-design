#include "usart.h"
#include "LED.h"
#include "irr.h"
#include "hil.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/6/23
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART1->TDR=(u8)ch;      
	return ch;
}
#endif 

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART句柄

// 变量声明
u8 USART3_RX_BUF[USART3_REC_LEN]; 
u8 USART3_RX_STA = 0;
u8 USART3_RxBuffer[USART3_RXBUFFERSIZE];
UART_HandleTypeDef USART3_Handler; //UART句柄
IRRtypeDef IRR;

//变量声明
u8 USART2_RX_BUF[USART2_REC_LEN]; 
u8 USART2_RX_STA = 0;
u8 USART2_RxBuffer[USART2_RXBUFFERSIZE];
UART_HandleTypeDef USART2_Handler;         //UART句柄
HILtypeDef HIL;


//COM3用于将收发流量计数据
 void AV87110_USART3_init(u32 bound)  
{
	USART3_Handler.Instance=USART3;					              // USART3
	USART3_Handler.Init.BaudRate=bound;				            // 波特率 9600
	USART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;    // 字长为8位数据格式
	USART3_Handler.Init.StopBits=UART_STOPBITS_1;	        // 一个停止位
	USART3_Handler.Init.Parity=UART_PARITY_NONE;          // UART_PARITY_EVEN;		//偶校验
	USART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;    // 无硬件流控
	USART3_Handler.Init.Mode=UART_MODE_TX_RX;		          // 收发模式
	HAL_UART_Init(&USART3_Handler);					              // HAL_UART_Init()会使能USART3
	
	HAL_UART_Receive_IT(&USART3_Handler, (u8 *)USART3_RxBuffer, USART3_RXBUFFERSIZE);
}

//COM2
void METV53USART2_init(u32 bound)
{
	USART2_Handler.Instance=USART2;					              // USART2
	USART2_Handler.Init.BaudRate=bound;				            // 波特率
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;    // 字长为8位数据格式
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;	        // 一个停止位
	USART2_Handler.Init.Parity=UART_PARITY_NONE;       		//无校验
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;    // 无硬件流控
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;		          // 收发模式
	HAL_UART_Init(&USART2_Handler);					              // HAL_UART_Init()会使能USART2
	
	HAL_UART_Receive_IT(&USART2_Handler, (u8 *)USART2_RxBuffer, USART2_RXBUFFERSIZE);
}


void uart_init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}


//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		
#if EN_USART1_RX
		//__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
#endif	
	}	
	
	if(huart->Instance==USART3)               //如果是串口3，进行串口3 MSP初始化
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();			      //使能GPIOB时钟
		__HAL_RCC_USART3_CLK_ENABLE();			    //使能USART3时钟
	
		GPIO_Initure.Pin=GPIO_PIN_10;			      //PB10 = TX
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		  //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			    //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		  //高速
		GPIO_Initure.Alternate=GPIO_AF7_USART3;	//复用为USART
		HAL_GPIO_Init(GPIOB, &GPIO_Initure);	   //初始化且选择GPIO分组为A组

		GPIO_Initure.Pin=GPIO_PIN_11;			       //PB11 = RX
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	   	 //初始化PB11
		
#if EN_USART3_RX	
        //__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断		
		HAL_NVIC_EnableIRQ(USART3_IRQn);		//使能USART2中断通道
		HAL_NVIC_SetPriority(USART3_IRQn,2,1);	//抢占优先级3，子优先级3
#endif			
	}
		if(huart->Instance==USART2)               //如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			      //使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE();			    //使能USART2时钟
	
		GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;			      //PA2 = TX
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		  //复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			    //上拉
		GPIO_Initure.Speed=GPIO_SPEED_HIGH;		  //高速
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//复用为USART
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   //初始化且选择GPIO分组为A组
		
		
#if EN_USART2_RX	
        //__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断		
		HAL_NVIC_EnableIRQ(USART2_IRQn);		//使能USART2中断通道
		HAL_NVIC_SetPriority(USART2_IRQn,2,1);	//抢占优先级3，子优先级3
#endif		
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	u8 rec;
	//u8 i;
	
	if(huart->Instance==USART1)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}
	
	if(huart->Instance==USART3) //如果是串口3
	{
		rec = *(--(huart->pRxBuffPtr));
		//printf("rec = %02X ", rec);
//		LED0_Toggle;                    // 接收到数据闪烁LED0
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
		if(huart->Instance==USART2) //如果是串口2
	{
		rec = *(--(huart->pRxBuffPtr));
		//printf("rec = %02X ", rec);
//		LED0_Toggle;                    // 接收到数据闪烁LED0
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
	

//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
    u32 maxDelay=0x1FFFF;

	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&UART1_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&UART1_Handler,(u8 *)aRxBuffer, RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}

} 


void USART3_IRQHandler(void)                	
{ 
	u32 timeout=0;
  u32 maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&USART3_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&USART3_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&USART3_Handler,(u8 *)USART3_RxBuffer, USART3_RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}

} 


void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
  u32 maxDelay=0x1FFFF;

	HAL_UART_IRQHandler(&USART2_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_UART_GetState(&USART2_Handler)!=HAL_UART_STATE_READY)//等待就绪
	{
        timeout++;////超时处理
        if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_UART_Receive_IT(&USART2_Handler,(u8 *)USART2_RxBuffer, USART2_RXBUFFERSIZE)!=HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
        timeout++; //超时处理
        if(timeout>maxDelay) break;	
	}
//	printf("b");

} 








 

 




