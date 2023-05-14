#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
//#include "Protocol.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//CAN��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
////////////////  //////////////////
#define CurrentSetMode 31
#define VoltageSetMode 30
#define ID_Voltage_DC (u8)25
#define ID_Current_DC (u8)26
#define ID_Warning_DC (u8)27

#define IdcMax 20 // DC�������õ�������Ϊ 25A
#define IdcMin 0
#define UdcMax (float)25.5
#define UdcMin 0


extern CAN_HandleTypeDef   CAN1_Handler;   //CAN1���
extern CanTxMsgTypeDef     TxMessage;      //������Ϣ
extern CanRxMsgTypeDef     RxMessage;      //������Ϣ

typedef struct
{
	float Voltage_in_DC;		//DC�����ѹ
	float Voltage_out_DC;		//DC�����ѹ
	float Temp_DC;				//DC�¶�
	float Current_in_DC;		//DC�������
	float Current_out_DC;		//DC�������
	float Warinning_DC;			//DC����

}DCDCMessage;

extern DCDCMessage    DCDC;
extern u8 CAN_RX_BUF[8];
extern u8 ID_FLAG;

//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN��ʼ��
u8 CAN1_Send_Msg(u8* msg,u8 len);						//��������
u8 CAN1_Receive_Msg(u8 *buf);							//��������

u8 CAN_Decode(u8 ID, u8 CAN_RX_BUF[8], DCDCMessage *CAN_Data);
void DCDC_CAN_Set(float CVcmd, u8 mode);
void DCDC_CAN_Init(float Idc0, float Udc0);


#endif


