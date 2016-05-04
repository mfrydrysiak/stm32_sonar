/**
  ******************************************************************************
  * @file    main.c
  * @author  Marek Frydrysiak
  * @version V1.0
  * @date    26.02.2016
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "global_util.h"

void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if (licznik > 0)
		{
			licznik--;
			flaga = 0;
		}
	}
}

int main()
{
	flaga = 1;
	//adc_ptr = adc_value;
	koniec = 0;
	current_angle_PWM = PWM_INITIAL_POS;

	hardware_config();		// configure specified peripherals

	while(1)
	{
		if (koniec != 1)
		{
			servo_sonar_control();
		}
	}
}
