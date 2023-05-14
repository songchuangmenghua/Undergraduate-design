#include "spi.h"
#include "delay.h"


void SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			      //����GPIOAʱ��
	
    GPIO_Initure.Pin= GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;   // ����ƬѡCS2,3,4����PA5,6,7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);     //��ʼ��GPIOA
	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);	//PA5��1 
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);	//PA6��1 
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);	//PA7��1 
	
    __HAL_RCC_GPIOB_CLK_ENABLE();			       //����GPIOBʱ��	
	  GPIO_Initure.Pin= GPIO_PIN_2|GPIO_PIN_15;// ����Dout�������ţCPB2��PB15
		GPIO_Initure.Mode=GPIO_MODE_INPUT;       //����Ϊ����
	
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_13;// ʱ���ź� PB7��PB8��PB9��PB13
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   // �������
    GPIO_Initure.Pull=GPIO_PULLUP;           //����
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;    //����
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB7��1�����ʼʱ���ź�Ϊ�ߵ�ƽ
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);	//PB8��1	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);	//PB9��1	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//PB13��1	
		
    
	
	  __HAL_RCC_GPIOC_CLK_ENABLE();			      //����GPIOCʱ��	
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4; // ����Dout��������:�PPC1��PC4
		GPIO_Initure.Mode=GPIO_MODE_INPUT;       //����Ϊ����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);		
	
//    GPIO_Initure.Pin= GPIO_PIN_5;           // CS1����PC5
		GPIO_Initure.Pin= GPIO_PIN_6;           // CS1����PC5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //��ʼ��GPIOC
//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	//PC5��1����0ʱ����
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);	//PC5��1����0ʱ����
		
}

double SPI_CV1() // PC6ΪƬѡ�źţ�PB7Ϊʱ���źţ�PB2Ϊ�����ź�
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB7(0);
	delay_ms(1);
	PC6(0);//Ƭѡ
	delay_us(1);
	for(i=0;i<16;++i)
	{
		data<<=1;
		PB7(1);
		delay_us(5);
		PB7(0);
		delay_us(1);
//		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_14)==SET )
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)==SET )
			{
				data++;
			}
			delay_us(4);
		}
	// У��
//		data1 = data;
//		data1 = data & 0x8000
//		if(data1 != 0){}
		
		data=data & 0x1FFF;
		data_float=data-4096;
		data_float=data_float*0.0125;
		PC6_Toggle;
		PC6(1);
		return data_float;
}

double SPI_CV2()//PA5ΪƬѡ�źţ�PB8Ϊʱ���źţ�PB15Ϊ�����ź�
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB8(0);
	delay_ms(1);
	PA5(0);//Ƭѡ
	delay_us(1);
	for(i=0;i<16;++i)
	{
		data<<=1;
		PB8(1);
		delay_us(5);
		PB8(0);
		delay_us(1);
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15)==SET )
			{
				data++;
			}
			delay_us(4);
		}
		data=data & 0x1FFF;
		data_float=data-4096;
		data_float=data_float*0.0125;
		PA5_Toggle;
		PA5(1);
		return data_float;
}

double SPI_CV3()//PA6ΪƬѡ�źţ�PB9Ϊʱ���źţ�PC1Ϊ�����ź�
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB9(0);
	delay_ms(1);
	PA6(0);//Ƭѡ
	delay_us(1);
	for(i=0;i<16;++i)
	{
		data<<=1;
		PB9(1);
		delay_us(5);
		PB9(0);
		delay_us(1);
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)==SET )
			{
				data++;
			}
			delay_us(4);
		}
		data=data & 0x1FFF;
		data_float=data-4096;
		data_float=data_float*0.0125;
		PA6_Toggle;
		PA6(1);
		return data_float;
}

double SPI_CV4()//PA7ΪƬѡ�źţ�PB13Ϊʱ���źţ�PC4Ϊ�����ź�
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB13(0);
	delay_ms(1);
	PA7(0);//Ƭѡ
	delay_us(1);
	for(i=0;i<16;++i)
	{
		data<<=1;
		PB13(1);
		delay_us(5);
		PB13(0);
		delay_us(1);
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)==SET )
			{
				data++;
			}
			delay_us(4);
		}
		data=data & 0x1FFF;
		data_float=data-4096;
		data_float=data_float*0.0125;
		PA7_Toggle;
		PA7(1);
		return data_float;
}


