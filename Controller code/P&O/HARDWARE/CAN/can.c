#include "can.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
  

CAN_HandleTypeDef   CAN1_Handler;   //CAN1���
CanTxMsgTypeDef     TxMessage;      //������Ϣ
CanRxMsgTypeDef     RxMessage;      //������Ϣ
DCDCMessage         DCDC;

u8 ID_FLAG;

////CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1TQ~CAN_SJW_4TQ
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1TQ~CAN_BS2_8TQ;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1TQ~CAN_BS1_16TQ
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_MODE_NORMAL,��ͨģʽ;CAN_MODE_LOOPBACK,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ54M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_11tq,6,CAN_MODE_LOOPBACK);
//������Ϊ:54M/((6+11+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 

u8 CAN1_Mode_Init(u32 tsjw,u32 tbs2,u32 tbs1,u16 brp,u32 mode)
{
    CAN_FilterConfTypeDef  CAN1_FilerConf;
    
    CAN1_Handler.Instance=CAN1; 
    CAN1_Handler.pTxMsg=&TxMessage;     //������Ϣ
    CAN1_Handler.pRxMsg=&RxMessage;     //������Ϣ
    CAN1_Handler.Init.Prescaler=brp;    //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN1_Handler.Init.Mode=mode;        //ģʽ���� 
    CAN1_Handler.Init.SJW=tsjw;         //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1TQ~CAN_SJW_4TQ
    CAN1_Handler.Init.BS1=tbs1;         //tbs1��ΧCAN_BS1_1TQ~CAN_BS1_16TQ
    CAN1_Handler.Init.BS2=tbs2;         //tbs2��ΧCAN_BS2_1TQ~CAN_BS2_8TQ
    CAN1_Handler.Init.TTCM=DISABLE;     //��ʱ�䴥��ͨ��ģʽ 
    CAN1_Handler.Init.ABOM=DISABLE;     //����Զ����߹���
    CAN1_Handler.Init.AWUM=DISABLE;     //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN1_Handler.Init.NART=ENABLE;      //��ֹ�����Զ����� 
    CAN1_Handler.Init.RFLM=DISABLE;     //���Ĳ�����,�µĸ��Ǿɵ� 
    CAN1_Handler.Init.TXFP=DISABLE;     //���ȼ��ɱ��ı�ʶ������ 
    if(HAL_CAN_Init(&CAN1_Handler)!=HAL_OK) return 1;   //��ʼ��
	
//	CAN1_FilerConf.FilterIdHigh=0X0320;     //16λID 
//    CAN1_FilerConf.FilterIdLow=0X0340;
//    CAN1_FilerConf.FilterMaskIdHigh=0X0360; //16λMASK
//    CAN1_FilerConf.FilterMaskIdLow=0X7D40;  
	CAN1_FilerConf.FilterIdHigh=0X7D20;     //16λID 
    CAN1_FilerConf.FilterIdLow=0X0F00;
    CAN1_FilerConf.FilterMaskIdHigh=0X1040; //16λMASK
    CAN1_FilerConf.FilterMaskIdLow=0X0300;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
    CAN1_FilerConf.FilterNumber= 0;          //������0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_16BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN1_FilerConf.BankNumber=14;
	
	HAL_CAN_Receive_IT(&CAN1_Handler,0); //HAL_CAN_Receive_IT(CAN_HandleTypeDef* hcan, uint8_t FIFONumber)
    /*
    CAN1_FilerConf.FilterIdHigh=0X0000;     //32λID
    CAN1_FilerConf.FilterIdLow=0X0000;
    CAN1_FilerConf.FilterMaskIdHigh=0X0000; //32λMASK
    CAN1_FilerConf.FilterMaskIdLow=0X0000;  
    CAN1_FilerConf.FilterFIFOAssignment=CAN_FILTER_FIFO0;//������0������FIFO0
    CAN1_FilerConf.FilterNumber=0;          //������0
    CAN1_FilerConf.FilterMode=CAN_FILTERMODE_IDMASK;
    CAN1_FilerConf.FilterScale=CAN_FILTERSCALE_32BIT;
    CAN1_FilerConf.FilterActivation=ENABLE; //�����˲���0
    CAN1_FilerConf.BankNumber=14;
	*/
    if(HAL_CAN_ConfigFilter(&CAN1_Handler,&CAN1_FilerConf)!=HAL_OK) return 2;//�˲�����ʼ��
    return 0;
}

//CAN�ײ��������������ã�ʱ�����ã��ж�����
//�˺����ᱻHAL_CAN_Init()����
//hcan:CAN���
void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_CAN1_CLK_ENABLE();                //ʹ��CAN1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			    //����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_11|GPIO_PIN_12;   //PA11,12
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //���츴��
    GPIO_Initure.Pull=GPIO_PULLUP;              //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;         //����
    GPIO_Initure.Alternate=GPIO_AF9_CAN1;       //����ΪCAN1
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);         //��ʼ��
    
#if CAN1_RX0_INT_ENABLE
    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//FIFO0��Ϣ�Һ��ж�����.	  
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn,2,2);    //��ռ���ȼ�1�������ȼ�2
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);          //ʹ���ж�
#endif	
}

#if CAN1_RX0_INT_ENABLE                         //ʹ��RX0�ж�
//CAN�жϷ�����
void CAN1_RX0_IRQHandler(void)
{
    HAL_CAN_IRQHandler(&CAN1_Handler);//�˺��������CAN_Receive_IT()��������
}

//CAN�жϴ������
//�˺����ᱻCAN_Receive_IT()����
//hcan:CAN���
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
    __HAL_CAN_ENABLE_IT(&CAN1_Handler,CAN_IT_FMP0);//���¿���FIF00��Ϣ�Һ��ж�
	
	ID_FLAG = CAN_Decode(CAN1_Handler.pRxMsg->StdId, CAN1_Handler.pRxMsg->Data, &DCDC); //u8 DCDC_CAN_Decode(u8 ID, u8 CAN_RX_BUF[8], DCDCTypeDef *CAN_DC);
	
}
#endif	

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
    u16 i=0;
		//CAN1_Handler.pTxMsg->StdId=0X12;        //��׼��ʶ�� ID =100 = 0x64 
    //CAN1_Handler.pTxMsg->ExtId=0x12;        //��չ��ʶ��(29λ) 
    CAN1_Handler.pTxMsg->StdId=0X64;        //��׼��ʶ��
    CAN1_Handler.pTxMsg->ExtId=0x00;        //��չ��ʶ��(29λ)
    CAN1_Handler.pTxMsg->IDE=CAN_ID_STD;    //ʹ�ñ�׼֡
    CAN1_Handler.pTxMsg->RTR=CAN_RTR_DATA;  //����֡
    CAN1_Handler.pTxMsg->DLC=len;                
    for(i=0;i<len;i++)
    CAN1_Handler.pTxMsg->Data[i]=msg[i];
    if(HAL_CAN_Transmit(&CAN1_Handler,10)!=HAL_OK) return 1;     //����
    return 0;		
}

//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
    if(HAL_CAN_Receive(&CAN1_Handler,CAN_FIFO0,0)!=HAL_OK) return 0;//��������	
    for(i=0;i<CAN1_Handler.pRxMsg->DLC;i++)
    buf[i]=CAN1_Handler.pRxMsg->Data[i];
	return CAN1_Handler.pRxMsg->DLC;	
}


// ����DCDC���Ƶĵ�ѹ��������
// ��������: CurrentSetMode = 31;
// ��ѹ����: VoltageSetMode = 30;
void DCDC_CAN_Set(float CVcmd, u8 mode)
{
	u8 canbuf[8] = {0,0,0,0,0,0,0,0};
	u8 res;
//	u8 I_CMD,U_CMD;
	
	canbuf[0] = mode;
	
	if(mode == CurrentSetMode)
	{
		if(CVcmd>=IdcMax){CVcmd = IdcMax;} // �������±߽�����
		else if(CVcmd<=IdcMin){CVcmd = IdcMin;}
//		I_CMD = Float2u8(CVcmd*10);
		canbuf[2] = CVcmd*10;
	}
	
	if(mode == VoltageSetMode)
	{
		if(CVcmd>=UdcMax){CVcmd = UdcMax;} // ��ѹ���±߽�����
		else if(CVcmd<=UdcMin){CVcmd = UdcMin;}
//		U_CMD = Float2u8(CVcmd);
		canbuf[2] = CVcmd*10;
	}	
	
	res = CAN1_Send_Msg(canbuf,8);//����8���ֽ� 
}


void DCDC_CAN_Init(float Idc0, float Udc0)
{
	
	DCDC_CAN_Set(Idc0, CurrentSetMode); // ������ʼ��
	delay_ms(100);
	DCDC_CAN_Set(Udc0, VoltageSetMode);	// ��ѹ��ʼ��
	delay_ms(100);
	
}

// ȼ�ϵ��ͨ������
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
		
			if(DC_Waring&0x0001){printf("���Ƿѹ");}
			if(DC_Waring&0x0002){printf("�����ѹ");}
			if(DC_Waring&0x0004){printf("�������");}
			if(DC_Waring&0x0008){printf("�������");}
			break;	
			
			
		default:
			break;
	}
	return ID;
	
}


