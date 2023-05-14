#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F7������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3PWM��� 
extern TIM_HandleTypeDef TIM2_Handler;      //��ʱ��3PWM��� 
extern TIM_HandleTypeDef TIM7_Handler;      //��ʱ��7���


extern u8 TIM2_FLAG;
extern u8 TIM3_FLAG;

//extern u32 TIM2_n;
//extern float TIM2_dt;

//extern double TIM2_tim;
//extern double TIM3_tim;

void TIM3_Init(u16 arr,u16 psc);    //��ʱ����ʼ��
void TIM2_Init(u16 arr,u16 psc);    //��ʱ����ʼ��
void TIM7_Init(u16 arr,u16 psc);

#endif

