/*
 * @Author: ������
 * @Date: 2022-04-03 00:00:43
 * @LastEditors: ������
 * @LastEditTime: 2022-10-02 16:41:16
 * @FilePath: \undefinedg:\Innovation and Entrepreneurship\Research on online control method of optimal power output for pure solar UAV\code\exp5all\Exp5test\HARDWARE\ACC\acc.c
 * @Description: ��Ҫ��������ص�֧�����ʼ�:debuking@qq.com
 * 
 * Copyright (c) 2022 by ������/��������ѧ�ѧԺ, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <acc.h>
#include "sdram.h"
throttle_optimal TO;

void initialize_val() //��ʼ������
{
	TO.Power = 0;
	TO.Power_temp = 0;
	TO.Power_max = 0;
	TO.accelerator = 0;
	TO.accelerator_temp = 0;
	TO.accelerator_max = 0;
	TO.delta_accelerator = 1;
}

// void get_P()				//��ȡ��ѹ���������㹦��
//{
//	TO.Power = current * voltage;
// }

u16 jud_P() //�Ŷ��۲ⷨ
{
	u16 acc_temp_temp;
	if (TO.Power == 0)
	{
		TO.accelerator = 20;
		//		TO.Power_temp = TO.Power;
		TO.accelerator_temp = TO.accelerator;
		//		TO.accelerator = TO.accelerator + TO.delta_accelerator;
	}
	else
	{
		if (TO.Power > TO.Power_temp)
		{
			if (TO.accelerator > TO.accelerator_temp)
			{
				TO.Power_temp = TO.Power;
				TO.accelerator_temp = TO.accelerator;
				TO.accelerator = TO.accelerator + TO.delta_accelerator;
			}
			else //(TO.accelerator<TO.accelerator_temp)
			{
				TO.Power_temp = TO.Power;
				TO.accelerator_temp = TO.accelerator;
				TO.accelerator = TO.accelerator - TO.delta_accelerator;
			}
		}
		if (TO.Power < TO.Power_temp)
		{
			if (TO.accelerator > TO.accelerator_temp)
			{
				TO.Power_temp = TO.Power;
				TO.accelerator_temp = TO.accelerator;
				TO.accelerator = TO.accelerator - TO.delta_accelerator;
			}
			else //(TO.accelerator<TO.accelerator_temp)
			{
				TO.Power_temp = TO.Power;
				TO.accelerator_temp = TO.accelerator;
				TO.accelerator = TO.accelerator + TO.delta_accelerator;
			}
		}
	}
	if (TO.accelerator < 100 || TO.accelerator > 0)
	{
		return TO.accelerator;
	}
	else if (TO.accelerator > 100)
	{
		return 100;
	}
	else
	{
		return 0;
	}
}
