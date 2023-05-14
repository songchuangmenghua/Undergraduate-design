#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
  

CAN_HandleTypeDef   CAN1_Handler;   //CAN1句柄
CanTxMsgTypeDef     TxMessage;      //发送消息
CanRxMsgTypeDef     RxMessage;      //接收消息
DCDCMessage         DCDC;

u8 ID_FLAG;

////CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_MODE_NORMAL,普通模式;CAN_MODE_LOOPBACK,回环模式;
//Fpclk1的时钟在初始化的时候设置为54M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_11tq,6,CAN_MODE_LOOPBACK);
//则波特率为:54M/((6+11+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance=CAN1; 
    CAN1_Handler.pTxMsg=&TxMessage;     //发送消息
    CAN1_Handler.pRxMsg=&RxMessage;     //接收消息
    CAN1_Handler.Init.Prescaler=brp;    //分频系数(Fdiv)为brp+1
    CAN1_Handler.Init.Mode=mode;        //模式设置 
    CAN1_Handler.Init.SJW=tsjw;         //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1范围CAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2范围CAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //非时间触发通信模式 
    CAN1_Handler.Init.ABOM=DISABLE;     //软件自动离线管理
    CAN1_Handler.Init.AWUM=DISABLE;     //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN1_Handler.Init.NART=ENABLE;      //禁止报文自动传送 
    CAN1_Handler.Init.RFLM=DISABLE;     //报文不锁定,新的覆盖旧的 
    CAN1_Handler.Init.TXFP=DISABLE;     //优先级由报文标识符决定 
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) return 1;   //初始化
	
//	CAN1_FilerConf.FilterIdHigh=0X0320;     //16位ID 
//    CAN1_FilerConf.FilterIdLow=0X0340;
//    CAN1_FilerConf.FilterMaskIdHigh=0X0360; //16位MASK
//    CAN1_FilerConf.FilterMaskIdLow=0X7D40;  
	CAN1_FilerConf.FilterIdHigh=0X7D20;     //16位ID 
    CAN1_FilerConf.FilterIdLow=0X0F00;
    CAN1_FilerConf.FilterMaskIdHigh=0X1040; //16位MASK
    CAN1_FilerConf.FilterMaskIdLow=0X0300;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
    CAN1_FilerConf.FilterNumber= 0;          //过滤器0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_16BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //激活滤波器0
    CAN1_FilerConf.BankNumber=14;
	
	HAL_CAN_Receive_IT(&CAN1_Handler,0); //HAL_CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)
    /*
    CAN1_FilerConf.FilterIdHigh=0X0000;     //32位ID
    CAN1_FilerConf.FilterIdLow=0X0000;
    CAN1_FilerConf.FilterMaskIdHigh=0X0000; //32位MASK
    CAN1_FilerConf.FilterMaskIdLow=0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
    CAN1_FilerConf.FilterNumber=0;          //过滤器0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //激活滤波器0
    CAN1_FilerConf.BankNumber=14;
	*/
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) return 2;//滤波器初始化
    return 0;
}

//CAN底层驱动，引脚配置，时钟配置，中断配置
//此函数会被HAL_CAN_Init()调用
//hcan:CAN句柄
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //使能CAN1时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();			    //开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;   //PA11,12
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //推挽复用
    GPIO_Initure.Pull=GPIO_PULLUP;              //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //快速
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //复用为CAN1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //初始化
    
#if CAN1_RX0_INT_ENABLE
    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//FIFO0消息挂号中断允许.	  
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn,2,2);    //抢占优先级1，子优先级2
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          //使能中断
#endif	
}

#if CAN1_RX0_INT_ENABLE                         //使能RX0中断
//CAN中断服务函数
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);//此函数会调用CAN_Receive_IT()接收数据
}

//CAN中断处理过程
//此函数会被CAN_Receive_IT()调用
//hcan:CAN句柄
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//重新开启FIF00消息挂号中断
	
	ID_FLAG = CAN_Decode(CAN1_Handler.pRxMsg->StdId, CAN1_Handler.pRxMsg->Data, &DCDC); //u8 DCDC_CAN_Decode(u8 ID, u8 CAN_RX_BUF[8], DCDCTypeDef *CAN_DC);
	
}
#endif	

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u16 i=0;
		//CAN1_Handler.pTxMsg->StdId=0X12;        //标准标识符 ID =100 = 0x64 
    //CAN1_Handler.pTxMsg->ExtId=0x12;        //扩展标识符(29位) 
    CAN1_Handler.pTxMsg->StdId=0X64;        //标准标识符
    CAN1_Handler.pTxMsg->ExtId=0x00;        //扩展标识符(29位)
    CAN1_Handler.pTxMsg->IDE=CAN_ID_STD;    //使用标准帧
    CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;  //数据帧
    CAN1_Handler.pTxMsg->DLC=len;                
    for(i=0;i<len;i++)
    CAN1_Handler.pTxMsg->Data[i]=msg[i];
    if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK) return 1;     //发送
    return 0;		
}

//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;//接收数据	
    for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
    buf[i]=CAN1_Handler.pRxMsg->Data[i];
	return CAN1_Handler.pRxMsg->DLC;	
}


// 设置DCDC限制的电压和最大电流
// 电流设置: CurrentSetMode = 31;
// 电压设置: VoltageSetMode = 30;
void DCDC_CAN_Set(float CVcmd, u8 mode)
{
	u8 canbuf[8] = {0,0,0,0,0,0,0,0};
	u8 res;
//	u8 I_CMD,U_CMD;
	
	canbuf[0] = mode;
	
	if(mode == CurrentSetMode)
	{
		if(CVcmd>=IdcMax){CVcmd = IdcMax;} // 电流上下边界限制
		else if(CVcmd<=IdcMin){CVcmd = IdcMin;}
//		I_CMD = Float2u8(CVcmd*10);
		canbuf[2] = CVcmd*10;
	}
	
	if(mode == VoltageSetMode)
	{
		if(CVcmd>=UdcMax){CVcmd = UdcMax;} // 电压上下边界限制
		else if(CVcmd<=UdcMin){CVcmd = UdcMin;}
//		U_CMD = Float2u8(CVcmd);
		canbuf[2] = CVcmd*10;
	}	
	
	res = CAN1_Send_Msg(canbuf,8);//发送8个字节 
}


void DCDC_CAN_Init(float Idc0, float Udc0)
{
	
	DCDC_CAN_Set(Idc0, CurrentSetMode); // 电流初始化
	delay_ms(100);
	DCDC_CAN_Set(Udc0, VoltageSetMode);	// 电压初始化
	delay_ms(100);
	
}

// 燃料电池通信驱动
u8 CAN_Decode(u8 ID, u8 CAN_RX_BUF[8], DCDCMessage *Data_CAN)
{
//		u16 FC_fan,FC_Temp,FC_Current;
		u16 DC_in_I,DC_out_I,DC_in_U,DC_out_U,DC_Temp,DC_Waring;
	//	u16 Bat_I,Bat_U;
	switch(ID)
	{
						
		case ID_Current_DC:			
			DC_out_I=CAN_RX_BUF[1]<<8|CAN_RX_BUF[0];
			DC_in_I=CAN_RX_BUF[3]<<8|CAN_RX_BUF[2];
			Data_CAN->Current_out_DC=(float)DC_out_I*0.1;
			Data_CAN->Current_in_DC=(float)DC_in_I*0.1;
			break;
		
		case ID_Voltage_DC:
			DC_in_U=CAN_RX_BUF[1]<<8|CAN_RX_BUF[0];
			DC_out_U=CAN_RX_BUF[3]<<8|CAN_RX_BUF[2];
			DC_Temp=CAN_RX_BUF[7]<<8|CAN_RX_BUF[6];		
			Data_CAN->Voltage_in_DC=(float)DC_in_U*0.01;
			Data_CAN->Voltage_out_DC=(float)DC_out_U*0.01;
			Data_CAN->Temp_DC=(float)DC_Temp*0.1;
			break;
	
		case ID_Warning_DC:
			DC_Waring  = (CAN_RX_BUF[1]<<8) | CAN_RX_BUF[0];
			Data_CAN->Warinning_DC = DC_Waring;
		
			if(DC_Waring&0x0001){printf("输出欠压");}
			if(DC_Waring&0x0002){printf("输出过压");}
			if(DC_Waring&0x0004){printf("输出过流");}
			if(DC_Waring&0x0008){printf("输出过温");}
			break;	
			
			
		default:
			break;
	}
	return ID;
	
}


