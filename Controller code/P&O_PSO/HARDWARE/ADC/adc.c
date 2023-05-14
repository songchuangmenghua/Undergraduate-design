#include "adc.h"
#include "delay.h"	

ADC_HandleTypeDef ADC1_Handler;//ADC���



//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=108/4=27MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                       //ɨ��ģʽ ENABLE
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;              //����������ģʽ ENABLE
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ1
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //��������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ����������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
	
				
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();		      	//����GPIOAʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_2|GPIO_PIN_3;   //PA0-4-2-3,��ӦCV1-CV4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
		ADC_ChannelConfTypeDef ADC1_ChanConf;
		u16 temp;
	
    ADC1_ChanConf.Channel = ch;
		ADC1_ChanConf.Rank = 1;
		ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		ADC1_ChanConf.Offset = 0;
		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
	
		return (u16)HAL_ADC_GetValue(&ADC1_Handler);		            //�������һ��ADC1�������ת����
	
//		if(ADC_FLAG_EOC!=HAL_ADC_GetState(&ADC1_Handler))
//		{		
//			temp = (u16)HAL_ADC_GetValue(&ADC1_Handler);	
//		}
//		return temp;	            //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_Adc_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
//		delay_ms(5);
	}
	return temp_val/times;
} 