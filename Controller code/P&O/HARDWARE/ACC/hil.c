#include "usart.h"
#include "hil.h"

// 2a¨º?¨¬¡§D??¡é???¨®
//  21¡Á¡é¨º¨ª
u8 Tension_clear_Request(void)
{
	u8 ReqCode[8];
	ReqCode[0] = 0xEB;
	ReqCode[1] = 0x90;
	ReqCode[2] = 0x01;
	ReqCode[3] = 0x00;
	ReqCode[4] = 0x00;
	ReqCode[5] = 0x00;
	ReqCode[6] = 0x00;
	ReqCode[7] = 0x7C;

	HAL_UART_Transmit(&USART2_Handler, ReqCode, 8, 100);
	while (__HAL_UART_GET_FLAG(&USART2_Handler, UART_FLAG_TC) != SET)
		;

	return (1);
}

u8 Throttle_close_Request(void)
{
	u8 ReqCode[8];
	ReqCode[0] = 0xEB;
	ReqCode[1] = 0x90;
	ReqCode[2] = 0x06;
	ReqCode[3] = 0x00;
	ReqCode[4] = 0x00;
	ReqCode[5] = 0x00;
	ReqCode[6] = 0x00;
	ReqCode[7] = 0x7C;

	HAL_UART_Transmit(&USART2_Handler, ReqCode, 8, 100);
	while (__HAL_UART_GET_FLAG(&USART2_Handler, UART_FLAG_TC) != SET)
		;

	return (1);
}

u8 Throttle_Open_Request(void)
{
	u8 ReqCode[8];
	ReqCode[0] = 0xEB;
	ReqCode[1] = 0x90;
	ReqCode[2] = 0x05;
	ReqCode[3] = 0x00;
	ReqCode[4] = 0x00;
	ReqCode[5] = 0x00;
	ReqCode[6] = 0x00;
	ReqCode[7] = 0x80;

	HAL_UART_Transmit(&USART2_Handler, ReqCode, 8, 100);
	while (__HAL_UART_GET_FLAG(&USART2_Handler, UART_FLAG_TC) != SET)
		;
	return (1);
}

// ThrottleDuty ?a¨®¨ª??¨¢?¡ê?¡¤??¡ì?a0-100
u8 Throttle_set_Request(u16 ThrottleDuty)
{
	u8 ReqCode[8];
	u8 j;
	u16 PWM = 0;
	ReqCode[0] = 0XEB; // set throttle
	ReqCode[1] = 0X90;
	ReqCode[2] = 0X10;
	ReqCode[3] = ThrottleDuty;
	ReqCode[4] = ThrottleDuty >> 8;
	PWM = ThrottleDuty * 10 + 1000; // ¨®¨ª??0-100¡ê?¨®3¨¦?¦Ì?1000-2000¡ê??¡äPWM2¡§¦Ì???¦Ì???¨º¡À??
	if (PWM > 2000)
		(PWM = 2000);
	if (PWM < 1000)
		(PWM = 1000);
	printf("PWM=%d \n\r", PWM);
	ReqCode[5] = PWM;
	ReqCode[6] = PWM >> 8;
	ReqCode[7] = 0X00;
	for (j = 0; j < 7; j++)
	{
		ReqCode[7] = ReqCode[7] + ReqCode[j]; // ¦Ì¨²8??¡Á??¨²?aD¡ê?¨¦??¡ê?2¨¦¨®?o¨ª?¨¬?¨¦
	}

	HAL_UART_Transmit(&USART2_Handler, ReqCode, 8, 100);
	while (__HAL_UART_GET_FLAG(&USART2_Handler, UART_FLAG_TC) != SET)
		;

	return (1);
}

// D-¨°¨¦?a??
HILtypeDef METV53_Decode(u8 *RecData)
{
	u32 Byte_thrust, Byte_torque, Byte_epower, Byte_current, Byte_voltage;
	int j, h, i;
	HILtypeDef HILData;
	long int temp, Sum_Check, temp_1, temp_2;
	for (j = 0; j < 70; j++) // data check;
	{
		if ((RecData[0] == 0xEB) && (RecData[1] == 0x90))
		{
			break;
		}
		else
		{
			temp = RecData[0];
			for (h = 0; h < 69; h++)
			{
				RecData[h] = RecData[h + 1];
			}
			RecData[h] = temp;
		}
	}

	Sum_Check = 0x0000;
	for (i = 0; i < 68; i++) // Sum_Check
	{
		Sum_Check = Sum_Check + RecData[i];
	}
	temp_1 = Sum_Check >> 8;
	temp_1 = temp_1 << 8;
	temp_1 = Sum_Check - temp_1;

	if ((RecData[68] == temp_1) && (RecData[69] == temp_2))
	{
		Byte_thrust = RecData[9]; // tension
		Byte_thrust = (Byte_thrust << 8) + RecData[8];
		Byte_thrust = (Byte_thrust << 8) + RecData[7];
		Byte_thrust = (Byte_thrust << 8) + RecData[6];
		HILData.thrust = Byte_thrust / 1000;

		Byte_torque = RecData[11]; // Torque
		Byte_torque = (Byte_torque << 8) + RecData[10];
		HILData.torque = Byte_torque / 1000;

		Byte_voltage = RecData[3]; // voltage
		Byte_voltage = (Byte_voltage << 8) + RecData[2];
		HILData.voltage = Byte_voltage / 100;

		Byte_current = RecData[5]; // current
		Byte_current = (Byte_current << 8) + RecData[4];
		HILData.current = Byte_current / 100;

		Byte_epower = RecData[27]; // epower
		Byte_epower = (Byte_epower << 8) + RecData[26];
		HILData.epower = Byte_epower / 100.0;
	}
	else
	{

		printf("Motor Data Error");
	}

	return HILData;
}
