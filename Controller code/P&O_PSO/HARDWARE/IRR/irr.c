#include "usart.h"
#include "delay.h"
#include "led.h"
#include "irr.h"




// AV87110A���նȼ���Ϣ����
u8 AV87110_RS232_Request(void)
{
	u8 ReqCode[12]; 
	ReqCode[0] = 0x55;
	ReqCode[1] = 0x00;
	ReqCode[2] = 0x07; // D3-D9�ĳ���
	ReqCode[3] = 0x61;
	ReqCode[4] = 0x00;
	ReqCode[5] = 0x00;
	ReqCode[6] = 0x00;
	ReqCode[7] = 0x00;	
	ReqCode[8] = 0x00;
	ReqCode[9] = 0x00;
	ReqCode[10] = 0x07+0x61; // D1-D9λ�ֽ���ֵ���֮��
	ReqCode[11] = 0xAA;
	
	HAL_UART_Transmit(&USART3_Handler,ReqCode,12,100);								//���ô���3��������
	while(__HAL_UART_GET_FLAG(&USART3_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���

	return(1);
}

// AV87110Э�����
IRRtypeDef AV87110_RS232_Decode(u8 *Data)
{
	int i;
	IRRtypeDef IRRdata;
	u16 IRRCRC = 0;
	if((Data[0] == 0x55)&&(Data[1]==0x00)&&(Data[2]==0x0C))
	{
		//for(i=1; i<15; i++){IRRCRC += Data[i];}
		IRRCRC = Data[1]+Data[2]+Data[3]+Data[4]+Data[5]+Data[6]+Data[7]+Data[8]+Data[9]+Data[10]+Data[11]+Data[12]+Data[13]+Data[14];
		if((IRRCRC&0x00FF) == Data[15])
		{
			IRRdata.Irradiance = ((Data[3]<<8) + Data[4]);                    // ���ն� W/m2
			IRRdata.PanelTemp  = ((Data[7]<<8) + Data[8])/100.00;             // ����¶� �棬�ó���100.00��������100������û��С���������
			IRRdata.AmbieTemp  = ((Data[9]<<8) + Data[10])/100.00;						// �����¶� �棬�ó���100.00��������100������û��С���������
			IRRdata.CurrentSC  = ((Data[11]<<8) + Data[12]);                  // ��·���� mA
			IRRdata.Angle      = ((((Data[13]<<8) + Data[14])&0xFFF0)>>4)/10; // ��ˮƽ��н�deg, ȡ��12λ
			IRRdata.VoltLow    = Data[14]&0x0F;                   						// �����Ƿ�Ƿѹ�� 1-������2-Ƿѹ��ȡ��4λ  
		}
	}

	return IRRdata;
}
