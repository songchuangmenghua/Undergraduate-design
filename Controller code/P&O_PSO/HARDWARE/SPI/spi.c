#include "spi.h"
#include "delay.h"


void SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();			      //开启GPIOA时钟
	
    GPIO_Initure.Pin= GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;   // 定义片选CS2,3,4引脚PA5,6,7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);     //初始化GPIOA
	
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);	//PA5置1 
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);	//PA6置1 
	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);	//PA7置1 
	
    __HAL_RCC_GPIOB_CLK_ENABLE();			       //开启GPIOB时钟	
	  GPIO_Initure.Pin= GPIO_PIN_2|GPIO_PIN_15;// 接收Dout数据引脚PB2、PB15
		GPIO_Initure.Mode=GPIO_MODE_INPUT;       //定义为输入
	
		GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_13;// 时钟信号 PB7、PB8、PB9、PB13
		GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;   // 推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;           //上拉
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;    //高速
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB7置1，输初始时钟信号为高电平
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET);	//PB8置1	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET);	//PB9置1	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);//PB13置1	
		
    
	
	  __HAL_RCC_GPIOC_CLK_ENABLE();			      //开启GPIOC时钟	
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_4; // 接收Dout数据引脚:PC1、PC4
		GPIO_Initure.Mode=GPIO_MODE_INPUT;       //定义为输入
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);		
	
//    GPIO_Initure.Pin= GPIO_PIN_5;           // CS1引脚PC5
		GPIO_Initure.Pin= GPIO_PIN_6;           // CS1引脚PC5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_MEDIUM;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);     //初始化GPIOC
//		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET);	//PC5置1，置0时计数
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);	//PC5置1，置0时计数
		
}

double SPI_CV1() // PC6为片选信号，PB7为时钟信号，PB2为数据信号
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB7(0);
	delay_ms(1);
	PC6(0);//片选
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
	// 校验
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

double SPI_CV2()//PA5为片选信号，PB8为时钟信号，PB15为数据信号
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB8(0);
	delay_ms(1);
	PA5(0);//片选
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

double SPI_CV3()//PA6为片选信号，PB9为时钟信号，PC1为数据信号
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB9(0);
	delay_ms(1);
	PA6(0);//片选
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

double SPI_CV4()//PA7为片选信号，PB13为时钟信号，PC4为数据信号
{
	u16 i,data=0;
	double data_float=0;
	delay_ms(1);
	PB13(0);
	delay_ms(1);
	PA7(0);//片选
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


