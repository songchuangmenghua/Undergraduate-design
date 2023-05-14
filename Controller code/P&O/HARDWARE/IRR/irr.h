#ifndef _IRR_H
#define _IRR_H

#include "usart.h"
#include "delay.h"

#define USART3_REC_LEN    16       // [55 00 0C 00 01 0A 28 09 DD 0A 06 00 00 00 A1 D6] ��ʵ��һ������16���ֽ�
#define EN_USART3_RX			1
#define USART3_RXBUFFERSIZE 1    // ���ջ���Ϊ8���ֽ�

extern u8 USART3_RxBuffer[USART3_RXBUFFERSIZE];//HAL��USART����Buffer��ÿ�ν�һ���ֽ�
extern u8 USART3_RX_BUF[USART3_REC_LEN]; //����һ������
extern u8 USART3_RX_STA;                 //���յ����ݵ��ֽ���
extern UART_HandleTypeDef USART3_Handler; //UART3���


typedef struct
{
	u16 Irradiance;		    // 87110̽ͷ�з��ն�: W/m2
	float CellTemp;		    // ���Ƭ�¶ȣ����ҵ��ԣ��û��������:
	float PanelTemp;			// ��ذ��¶� : 1/100��
	float AmbieTemp;      // �����¶�   ��1/100��
	u16 CurrentSC;   		  // ��·���� : mA
	u8  VoltLow;		      // Ƿѹ 1-������0-Ƿѹ
	u8 Angle;			        // ��б��
}IRRtypeDef;

extern IRRtypeDef IRR; 

void AV87110_USART3_init(u32 bound);
u8 AV87110_RS232_Request(void);
IRRtypeDef AV87110_RS232_Decode(u8 *Data);


#endif
