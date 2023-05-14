#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define PC6(n)		(n?HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET))
#define PC6_Toggle (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6)) //PC6�����ƽ��ת,CS1	   													 
#define PA5(n)		(n?HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET))
#define PA5_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5)) //PA1�����ƽ��ת,CS2	 
#define PA6(n)		(n?HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET))
#define PA6_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6)) //PA7�����ƽ��ת,CS3	 
#define PA7(n)		(n?HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET))
#define PA7_Toggle (HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7)) //PA7�����ƽ��ת,CS4	 

//����ʱ�����ɺ���
#define PB7(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET))
#define PB8(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET))
#define PB9(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET))
#define PB13(n)		(n?HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET))

void SPI_Init(void); //SPI��ʼ������
double SPI_CV1(void);	//��ȡPB14��������
double SPI_CV2(void);	//��ȡPB15��������
double SPI_CV3(void);	//��ȡPC1��������
double SPI_CV4(void);	//��ȡPC4��������
		 
#endif

