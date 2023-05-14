#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define PC6(n)		(n?HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET))
#define PC6_Toggle (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6)) //PC6输出电平翻转,CS1	   													 
#define PA5(n)		(n?HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET))
#define PA5_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)) //PA1输出电平翻转,CS2	 
#define PA6(n)		(n?HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET))
#define PA6_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6)) //PA7输出电平翻转,CS3	 
#define PA7(n)		(n?HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET))
#define PA7_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7)) //PA7输出电平翻转,CS4	 

//定义时钟生成函数
#define PB7(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET))
#define PB8(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET))
#define PB9(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET))
#define PB13(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET))

void SPI_Init(void); //SPI初始化函数
double SPI_CV1(void);	//读取PB14引脚数据
double SPI_CV2(void);	//读取PB15引脚数据
double SPI_CV3(void);	//读取PC1引脚数据
double SPI_CV4(void);	//读取PC4引脚数据
		 
#endif

