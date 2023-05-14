#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
//#include "Protocol.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//CAN驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
////////////////  //////////////////
#define CurrentSetMode 31
#define VoltageSetMode 30
#define ID_Voltage_DC (u8)25
#define ID_Current_DC (u8)26
#define ID_Warning_DC (u8)27

#define IdcMax 20 // DC允许设置的最大电流为 25A
#define IdcMin 0
#define UdcMax (float)25.5
#define UdcMin 0


extern CAN_HandleTypeDef   CAN1_Handler;   //CAN1句柄
extern CanTxMsgTypeDef     TxMessage;      //发送消息
extern CanRxMsgTypeDef     RxMessage;      //接收消息

typedef struct
{
	float Voltage_in_DC;		//DC输入电压
	float Voltage_out_DC;		//DC输出电压
	float Temp_DC;				//DC温度
	float Current_in_DC;		//DC输入电流
	float Current_out_DC;		//DC输出电流
	float Warinning_DC;			//DC警告

}DCDCMessage;

extern DCDCMessage    DCDC;
extern u8 CAN_RX_BUF[8];
extern u8 ID_FLAG;

//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode);//CAN初始化
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据
u8 CAN1_Receive_Msg(u8 *buf);							//接收数据

u8 CAN_Decode(u8 ID, u8 CAN_RX_BUF[8], DCDCMessage *CAN_Data);
void DCDC_CAN_Set(float CVcmd, u8 mode);
void DCDC_CAN_Init(float Idc0, float Udc0);


#endif


