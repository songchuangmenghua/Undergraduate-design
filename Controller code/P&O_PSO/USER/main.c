/*
 * @Author: debuking
 * @Date: 2023-05-15 01:14:35
 * @LastEditors: debuking
 * @LastEditTime: 2023-05-15 01:15:05
 * @FilePath: \undefinede:\Courses\毕业设计\GitHub\Controller code\P&O_PSO\USER\main.c
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

int main(void)
{
	u16 x_right, y0, dy, x_left;
	u16 i;

	u8 key;

	float TIM3_dt = 1, TIM2_dt = 0.5;
	double TIM3_tim = 0, TIM2_tim = 0;
	u32 n3 = 0, n2 = 0;
	u32 t = 1000;
	u32 t1 = 0;
	int cnt;
	float Voltage = 0;
	float Current;
	float Power1;
	u16 ThrottleDuty = 0;

	Cache_Enable();
	HAL_Init();
	Stm32_Clock_Init(432, 25, 2, 9);
	delay_init(216);
	uart_init(115200);
	METV53USART2_init(57600);
	LED_Init();
	SDRAM_Init();
	LCD_Init();
	TIM3_Init(10000 - 1, 10800 - 1);
	TIM2_Init(5000 - 1, 10800 - 1);
	KEY_Init();
	delay_ms(50);
	initialize_val();
	delay_ms(t);
	i = 0;
	cnt = 0;

	x_left = 30 + 1;
	y0 = 10;
	dy = 20;
	x_right = 130 + 1;
	LCD_Auto_Placement(x_left - 20, y0, dy, x_right);
	POINT_COLOR = BLUE;

	while (1)
	{

		key = KEY_Scan(0);
		if (key)
		{
			switch (key)
			{
			case KEY2_PRES:
				Throttle_Open_Request();
				Throttle_set_Request(25);
				delay_ms(100);
				while (key != KEY1_PRES)
				{
					key = KEY_Scan(0);

					if (TIM2_FLAG == 1)
					{

						if (cnt == 2)
						{
							u16 temp;
							if (n2 < (0XFFFFFFFF - 1))
							{
								TIM2_tim = (n2++) * TIM2_dt;
							}
							else
							{
								n2 = 0;
							}

							jud_P(i);
							printf("psotime=%f ", TIM2_tim);
							printf("power=%f ", TO.Powermax);
							printf("current=%f ", HIL.current);
							printf("voltage=%f ", HIL.voltage);

							printf("acc=%d \n\r", TO.acc);

							i++;
							if (i == 3)

							{
								i = 0;
							}
						}
						if (cnt == 1)
						{
							if (TIM2_tim < 10)
							{
								Throttle_set_Request(TO.accelerator[i]);
								delay_ms(250);
							}
							else
							{
								Throttle_set_Request(TO.acc);
								delay_ms(250);
							}
						}

						else
						{

							delay_ms(100);
							TIM2_FLAG = 0;
							if (cnt == 2)
							{
								cnt = 0;
							}
						}
					}
				}

				Throttle_close_Request();
				break;
			case KEY1_PRES:
				Throttle_close_Request();
				break;
			}
		}

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
}
