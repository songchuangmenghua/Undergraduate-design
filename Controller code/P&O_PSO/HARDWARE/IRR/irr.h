#ifndef _IRR_H
#define _IRR_H

#include "usart.h"
#include "delay.h"

#define USART3_REC_LEN    16       // [55 00 0C 00 01 0A 28 09 DD 0A 06 00 00 00 A1 D6] 真实的一包数据16个字节
#define EN_USART3_RX			1
#define USART3_RXBUFFERSIZE 1    // 接收缓存为8个字节

extern u8 USART3_RxBuffer[USART3_RXBUFFERSIZE];//HAL库USART接收Buffer，每次接一个字节
extern u8 USART3_RX_BUF[USART3_REC_LEN]; //接收一包数据
extern u8 USART3_RX_STA;                 //接收到数据的字节数
extern UART_HandleTypeDef USART3_Handler; //UART3句柄


typedef struct
{
	u16 Irradiance;		    // 87110探头盒辐照度: W/m2
	float CellTemp;		    // 电池片温度，厂家调试，用户不用输出:
	float PanelTemp;			// 电池板温度 : 1/100度
	float AmbieTemp;      // 环境温度   ：1/100度
	u16 CurrentSC;   		  // 短路电流 : mA
	u8  VoltLow;		      // 欠压 1-正常，0-欠压
	u8 Angle;			        // 倾斜角
}IRRtypeDef;

extern IRRtypeDef IRR; 

void AV87110_USART3_init(u32 bound);
u8 AV87110_RS232_Request(void);
IRRtypeDef AV87110_RS232_Decode(u8 *Data);


#endif
