/*
 * @Author: debuking
 * @Date: 2023-05-15 01:02:14
 * @LastEditors: debuking
 * @LastEditTime: 2023-05-15 01:12:59
 * @FilePath: \USER\main.c
 * @Description: For support related to this code, please contact:debuking@qq.com
 * 
 * Copyright (c) 2023 by debuking, School of Astronautics and Astronautics, Beijing University of Technology, All Rights Reserved. 
 */
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "can.h"
#include "timer.h"
#include "spi.h"
#include "adc.h"
#include "math.h"
#include "sdram.h"
#include "lcd.h"
#include "irr.h"
#include "dac.h"
#include "hil.h"
#include <stdio.h>
#include <stdlib.h>
#include <acc.h>
#include "key.h"

/************************************************
************************************************/
// HILtypeDef HILData;

int main(void)
{
	u16 x_right, y0, dy, x_left; // LCD����ʾ
	int i;
	u8 key;
	float temp;
	//		u16 acc;
	float TIM3_dt = 1, TIM2_dt = 0.5;
	double TIM3_tim = 0, TIM2_tim = 0;
	u32 n3 = 0, n2 = 0;
	u32 t = 1000;
	u32 t1 = 0;
	int cnt;
	// u8  a = 0;
	float Voltage = 0;
	float Current;
	float Power1;
	u16 ThrottleDuty = 0;

	
	Cache_Enable();					 //��L1-Cache
	HAL_Init();						 //��ʼ��HAL��
	Stm32_Clock_Init(432, 25, 2, 9); //����ʱ��,216Mhz
	delay_init(216);				 //��ʱ��ʼ��
	uart_init(115200);				 //����1ͨ�Ų�����Ϊ115200
	METV53USART2_init(57600);		 //�����ʵ�����9600
	LED_Init();						 // LED�Ƴ�ʼ��
	SDRAM_Init();					 //��ʼ��SDRAM
	LCD_Init();						 // LCD����ʼ��
	TIM3_Init(10000 - 1, 10800 - 1); // 1Hz = 10000Hz/10000; ��ʱ��3��ʼ������ʱ��ʱ��Ϊ108M����Ƶϵ��Ϊ10800-1����ʱ��3��Ƶ��Ϊ1Hz.
	TIM2_Init(5000 - 1, 10800 - 1);	 // 2Hz=10000Hz/5000;
	KEY_Init();						 //������ʼ��
	delay_ms(50);
	initialize_val();
	delay_ms(t);

	x_left = 30 + 1;
	y0 = 10;
	dy = 20;
	x_right = 130 + 1;
	LCD_Auto_Placement(x_left - 20, y0, dy, x_right);
	POINT_COLOR = BLUE;
	//		while(1)
	//		{
	//			if(TIM2_FLAG==1)
	//			{
	//				LCD_FLOAT_Show(x_left,y0+dy*1,HIL.thrust);				LCD_FLOAT_Show(x_right+20,y0+dy*1,HIL.torque);
	//				LCD_FLOAT_Show(x_left,y0+dy*2,HIL.voltage);				LCD_FLOAT_Show(x_right+20,y0+dy*2,HIL.current);
	//				LCD_FLOAT_Show(x_left,y0+dy*3,HIL.epower);
	//
	//				TIM2_FLAG=0;
	//			}
	//
	//		}
	//*

	//			for(i=0;i<70;i++)
	//			{
	//				printf("%x",USART2_RX_BUF[i]);
	// printf("\n");
	//			}

	//*/
	//		Throttle_Open_Request();

	//		printf("c");
	//		while(1)
	//		{
	//			Throttle_Open_Request();
	//				acc=10;
	//			delay_ms(5000);

	// Throttle_Open_Request();
	// while(1)
	//{
	//
	//	Throttle_set_Request(50);
	//	if(TIM2_FLAG==1)
	//	{
	//		LCD_FLOAT_Show(x_left,y0+dy*1,HIL.thrust);				LCD_FLOAT_Show(x_right+20,y0+dy*1,HIL.torque);
	//		LCD_FLOAT_Show(x_left,y0+dy*2,HIL.voltage);				LCD_FLOAT_Show(x_right+20,y0+dy*2,HIL.current);
	//		LCD_FLOAT_Show(x_left,y0+dy*3,HIL.epower);
	//
	//		TIM2_FLAG=0;
	//	}
	// }

	while (1)
	{

		//		while(1)
		key = KEY_Scan(0); //�õ���ֵ
		if (key)
		{
			switch (key)
			{
			case KEY2_PRES:
				Throttle_Open_Request();
				while (key != KEY1_PRES)
				{
					key = KEY_Scan(0);
					if (TIM2_FLAG == 1)
					{
						//								TO.Power=HIL.current*HIL.voltage;
						cnt++;
						if (cnt == 1)
						{
							if (n2 < (0XFFFFFFFF - 1))
							{
								TIM2_tim = (n2++) * TIM2_dt;
							}
							else
							{
								n2 = 0;
							}
							printf("time=%f ", TIM2_tim);
							printf("power=%f ", TO.Power);
							//									printf("epower=%f ",HIL.epower);
							printf("current=%f ", HIL.current);
							printf("voltage=%f ", HIL.voltage);
							printf("acc=%d \n\r", TO.accelerator);
						}
						else
						{
							TO.Power = HIL.current * HIL.voltage;
							//									printf("power=%f ",TO.Power);
							Throttle_set_Request(jud_P());
							//									printf("acc=%d \n\r",TO.accelerator);
							TIM2_FLAG = 0;
							cnt = 0;
						}
					}

					//			Throttle_set_Request(20);
					//							delay_ms(1000);
					//							Throttle_set_Request(jud_P());
					//							Throttle_set_Request(50);
					//							delay_ms(1000);
					//							if (TO.accelerator==0)
					//							{
					//								Throttle_set_Request(10);
					//								delay_ms(100);
					//								Throttle_set_Request(20);
					//								delay_ms(100);
					//							}

					//	Throttle_set_Request(jud_P());
					//							delay_ms(200);
					//							TO.Power=HIL.current*HIL.voltage;
					//							printf("power=%f \n\r",TO.Power);
					//							printf("epower=%f \n\r",HIL.epower);
					//							printf("current=%f \n\r",HIL.current);
					//							printf("voltage=%f \n\r",HIL.voltage);
					//							delay_ms(1000);
					////							Throttle_set_Request(TO.accelerator+10);
					//							delay_ms(1500);
				}
				//						Tension_clear_Request();
				Throttle_close_Request();
				break;
			case KEY1_PRES:
				Throttle_close_Request();
				break;
				//						case KEY0_PRES:
				//						Throttle_set_Request(jud_P());
				//						break;
			}
		}

		//			}
		//				Throttle_set_Request(jud_P());
		//				delay_ms(2000);
		//				acc=acc+10;
		//				if(acc>100)
		//				{
		//					acc=100;
		//				}
		if (TIM2_FLAG == 1)
		{
			LCD_FLOAT_Show(x_left, y0 + dy * 1, HIL.thrust);
			LCD_FLOAT_Show(x_right + 20, y0 + dy * 1, HIL.torque);
			LCD_FLOAT_Show(x_left, y0 + dy * 2, HIL.voltage);
			LCD_FLOAT_Show(x_right + 20, y0 + dy * 2, HIL.current);
			LCD_FLOAT_Show(x_left, y0 + dy * 3, HIL.epower);

			TIM2_FLAG = 0;
		}
	}

	////			delay_ms(5000);

	//			Throttle_close_Request();

	////
	//					printf("%x",);
	//				while(1)
	//				{
	//
	//					Voltage=HILData.voltage;
	//					printf("%f",HIL.voltage);
	//
	//					Current=HILData.current;
	//
	//					TO.Power=Current*Voltage;
	//
	//				}
	//					printf("  Current=%f",Current);
	//					printf("  Voltage=%f",Voltage);
	//					printf("  TO.Power=%f",TO.Power);
	//					printf("b");

	//
	//		while(1)
	//			{		key=KEY_Scan(0); 		    //�õ���ֵ
	//				if(key)
	//				{
	//					switch(key)
	//					{
	//
	//						case KEY2_PRES:
	//						Throttle_Open_Request();
	//						Tension_clear_Request();
	//						Throttle_set_Request(jud_P());
	//						break;
	//						case KEY1_PRES:
	//						Throttle_close_Request();
	//						break;
	//						case KEY0_PRES:
	//
	//						break;
	//					}
	//				}
	//			}
	//
	//
	/*		while(key==KEY2_PRES)
			{
				key=KEY_Scan(0);
//				temp=kbhit();
//				if(temp==)
//				{
//					goto
//				}
				if(TIM2_FLAG = 1)   //ÿ0.1��ɼ�һ��
				{
				if(n2 < (0xFFFFFFFF-1)){ TIM2_tim = (n2++)*TIM2_dt; }

				Current=HILData.current;
				Voltage=HILData.voltage;
				TO.Power=Current*Voltage;

				printf("  Current=%f",Current);
				printf("  Voltage=%f",Voltage);
				printf("  TO.Power=%f",TO.Power);

				ThrottleDuty = jud_P();    							//����Ѱ��

				Throttle_set_Request(ThrottleDuty);

				TIM2_FLAG=0;
				}
			}
			*/
}
