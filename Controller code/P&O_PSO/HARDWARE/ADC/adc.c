#include "adc.h"
#include "delay.h"	

ADC_HandleTypeDef ADC1_Handler;//ADC¾ä±ú



//³õÊ¼»¯ADC
//ch: ADC_channels 
//Í¨µÀÖµ 0~16È¡Öµ·¶Î§Îª£ºADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4·ÖÆµ£¬ADCCLK=PCLK2/4=108/4=27MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12Î»Ä£Ê½
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //ÓÒ¶ÔÆë
    ADC1_Handler.Init.ScanConvMode=DISABLE;                       //É¨ÃèÄ£Ê½ ENABLE
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //¹Ø±ÕEOCÖÐ¶Ï
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //¹Ø±ÕÁ¬Ðø×ª»»
    ADC1_Handler.Init.NbrOfConversion=1;                         //1¸ö×ª»»ÔÚ¹æÔòÐòÁÐÖÐ Ò²¾ÍÊÇÖ»×ª»»¹æÔòÐòÁÐ1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;              //²»Á¬Ðø²ÉÑùÄ£Ê½ ENABLE
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //²»Á¬Ðø²ÉÑùÍ¨µÀÊýÎª1
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //Èí¼þ´¥·¢
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//Ê¹ÓÃÈí¼þ´¥·¢
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //¹Ø±ÕDMAÇëÇó
    HAL_ADC_Init(&ADC1_Handler);                                 //³õÊ¼»¯ 
	
				
}

//ADCµ×²ãÇý¶¯£¬Òý½ÅÅäÖÃ£¬Ê±ÖÓÊ¹ÄÜ
//´Ëº¯Êý»á±»HAL_ADC_Init()µ÷ÓÃ
//hadc:ADC¾ä±ú
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //Ê¹ÄÜADC1Ê±ÖÓ
    __HAL_RCC_GPIOA_CLK_ENABLE();		      	//¿ªÆôGPIOAÊ±ÖÓ
	
		GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_2|GPIO_PIN_3;   //PA0-4-2-3,¶ÔÓ¦CV1-CV4
		GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //Ä£Äâ
    GPIO_Initure.Pull=GPIO_NOPULL;          //²»´øÉÏÏÂÀ­
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//»ñµÃADCÖµ
//ch: Í¨µÀÖµ 0~16£¬È¡Öµ·¶Î§Îª£ºADC_CHANNEL_0~ADC_CHANNEL_16
//·µ»ØÖµ:×ª»»½á¹û
u16 Get_Adc(u32 ch)   
{
		ADC_ChannelConfTypeDef ADC1_ChanConf;
		u16 temp;
	
    ADC1_ChanConf.Channel = ch;
		ADC1_ChanConf.Rank = 1;
		ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		ADC1_ChanConf.Offset = 0;
		HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);
	
    HAL_ADC_Start(&ADC1_Handler);                               //¿ªÆôADC
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //ÂÖÑ¯×ª»»
	
		return (u16)HAL_ADC_GetValue(&ADC1_Handler);		            //·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹
	
//		if(ADC_FLAG_EOC!=HAL_ADC_GetState(&ADC1_Handler))
//		{		
//			temp = (u16)HAL_ADC_GetValue(&ADC1_Handler);	
//		}
//		return temp;	            //·µ»Ø×î½üÒ»´ÎADC1¹æÔò×éµÄ×ª»»½á¹û
}
//»ñÈ¡Ö¸¶¨Í¨µÀµÄ×ª»»Öµ£¬È¡times´Î,È»ºóÆ½¾ù 
//times:»ñÈ¡´ÎÊý
//·µ»ØÖµ:Í¨µÀchµÄtimes´Î×ª»»½á¹ûÆ½¾ùÖµ
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
