#ifndef _ACC_H
#define _ACC_H
#include "sdram.h"

typedef struct
{
	//��ѹ����
	float Power;		   //���㹦��
	float Power_temp;	   //���湦��
	float Power_max;	   //���������
	u16 accelerator;	   //����
	u16 accelerator_temp;  //��������
	u16 accelerator_max;   //�����������
	u16 delta_accelerator; //���ű仯����
						   //�ж�
} throttle_optimal;		   //�ṹ������������Ѱ��

void initialize_val(); //��ʼ������
u16 jud_P();		   //�Ŷ��۲ⷨ
float get_P();		   //��ȡ��ѹ���������㹦��

extern throttle_optimal TO;

#endif
