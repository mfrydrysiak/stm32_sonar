/*
 * 	sonar_ctrl.c
 *
 *  Created on: 2 mar 2016
 *      Author: Marek Frydrysiak
 */

#include "stm32f10x.h"
#include "global_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void SysTick_Handler()
{
	if (timer_ms)
		timer_ms--;
}

void delay_ms(int time)
{
	timer_ms = time;
	while (timer_ms);
}

int __io_putchar(int c)
{
	send_char(c);
	return c;
}

void send_char(char c)
{
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, c);
}

void send_string(const char* s)
{
	while (*s)
		send_char(*s++);
}

void servo_sonar_control()
{
	long int newInitAngle;
	long int newFinalAngle;
	long int newStep;

	static uint16_t initAngle_PWM = PWM_INITIAL_POS;
	static uint16_t finalAngle_PWM = PWM_FINAL_POS;
	static uint16_t step_PWM = PWM_INIT_STEP;

	char initial_angle[] 	= "   ";
	char final_angle[] 		= "   ";
	char stepper[]			= "   ";

	char *ia_ptr = initial_angle;
	char *fa_ptr = final_angle;
	char *st_ptr = stepper;
	char current_char;

	uint8_t valIndex;

	while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));	// wait for data

	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
	{

		char c = USART_ReceiveData(USART2);

		switch (c)
		{
			/* Single data acquisition without rotation */
			case 's':
				while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
				if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
				{
					c = USART_ReceiveData(USART2);
					switch (c)
					{
						case 'l':
							data_acquisition(0);	// 0 - left sensor
							break;
						case 'r':
							data_acquisition(1);	// 1 - right sensor
							break;
						default:
							break;
					}
				}
				break;

			case 'c':
				while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
				if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
				{
					c = USART_ReceiveData(USART2);
					switch (c)
					{
						case 'l':
							data_acquisition(0);	// 0 - left sensor
							break;
						case 'r':
							data_acquisition(1);	// 1 - right sensor
							break;
						case '+':
							if ((current_angle_PWM + step_PWM) <= finalAngle_PWM)
							{
								current_angle_PWM += step_PWM;
								TIM4->CCR3 = current_angle_PWM;
							}
							else
							{
								current_angle_PWM = initAngle_PWM;
								TIM4->CCR3 = current_angle_PWM;
							}
							break;
						default:
							break;
					}
				}
				break;

			/* Change initial and/or final angle and/or step */
			case 'a':
				for (valIndex = 0; valIndex < 3; valIndex++)
				{
					current_char = 'x';
					while (current_char != '\n')
					{
						while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));	// wait for data
						if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
						{
							switch (valIndex)
							{
								case 0:
									*ia_ptr = USART_ReceiveData(USART2);
									current_char = *ia_ptr;
									if (*ia_ptr != '\n')
										ia_ptr++;
									break;

								case 1:
									*fa_ptr = USART_ReceiveData(USART2);
									current_char = *fa_ptr;
									if (*fa_ptr != '\n')
										fa_ptr++;
									break;

								case 2:
									*st_ptr = USART_ReceiveData(USART2);
									current_char = *st_ptr;
									if (*st_ptr != '\n')
										st_ptr++;
									break;

								default:
									break;
							}
						}
					}
				}
				newInitAngle = strtol(initial_angle, NULL, 10);
				newFinalAngle = strtol(final_angle, NULL, 10);
				newStep = strtol(stepper, NULL, 10);

				if (newInitAngle >= 0)
					initAngle_PWM = (uint16_t)(((float)(newInitAngle) / 90) * 900 + 1500);
				else
					initAngle_PWM = (uint16_t)(1500 - (((float)(abs(newInitAngle)) / 90) * 900));

				TIM4->CCR3 = initAngle_PWM;
				current_angle_PWM = initAngle_PWM;

				if (newFinalAngle >= 0)
					finalAngle_PWM = (uint16_t)(((float)(newFinalAngle) / 90) * 900 + 1500);
				else
					finalAngle_PWM = (uint16_t)(1500 - (((float)(abs(newFinalAngle)) / 90) * 900));

				step_PWM = (uint16_t)newStep;
				break;

			default:
				//koniec = 1;
				break;
		}
	}
}

void data_acquisition(short sNum)
{
	ptr_adcTab = adcDMA;

	switch (sNum)
	{
		case 0:
			adc_ptr = &adc_value[0];
			ENABLE_SENSOR_1_LEFT;
			break;
		case 1:
			adc_ptr = &adc_value[1];
			ENABLE_SENSOR_2_RIGHT;
			break;
		default:
			break;
	}

	delay_ms(20);

	licznik = MAX_DATA;

	/* Odczytuj dane z czujnika co 25us az do wyzerowania zmiennej 'licznik' */
	while(licznik)
	{
		flaga = 1;
		*(ptr_adcTab++) = *adc_ptr;
		while(flaga);
	}

	switch (sNum)
	{
		case 0:
			DISABLE_SENSOR_1_LEFT;
			break;
		case 1:
			DISABLE_SENSOR_2_RIGHT;
			break;
		default:
			break;
	}

	ptr_adcTab = adcDMA; //powrot ustawienia wskaznika na poczatek tablicy
	uint16_t i;

	for (i = 0; i < MAX_DATA; i++)
	{
		printf("%d\n",*ptr_adcTab);
		ptr_adcTab++;
	}
	delay_ms(50);
}
