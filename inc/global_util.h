/*
 * global_util.h
 *
 *  Created on: 26 lut 2016
 *      Author: Marek Frydrysiak
 */

#ifndef GLOBAL_UTIL_H_
#define GLOBAL_UTIL_H_

// Global defines
#define		PWM_PERIOD_20MS			(20000 - 1)
#define		PWM_TIM_PRESCALER		(64 - 1)
#define		PWM_INITIAL_POS			1050
#define		PWM_FINAL_POS			1950
#define 	PWM_INIT_STEP			10
#define		MAX_DATA				800

#define 	ENABLE_SENSOR_1_LEFT	GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define 	DISABLE_SENSOR_1_LEFT	GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define 	ENABLE_SENSOR_2_RIGHT	GPIO_SetBits(GPIOC, GPIO_Pin_11)
#define 	DISABLE_SENSOR_2_RIGHT	GPIO_ResetBits(GPIOC, GPIO_Pin_11)

// Configuration functions
void hardware_config(void);
void servo_sonar_control(void);
void data_acquisition(short sNum);
void send_char(char c);
void send_string(const char* s);
int __io_putchar(int c);

// Global variables
volatile uint16_t 	adc_value[2];	// Stores the last ADC values (DMA) from 2 channels
volatile uint16_t 	*adc_ptr;
volatile uint16_t 	licznik;
volatile uint8_t	flaga;
volatile uint32_t 	timer_ms;

volatile uint16_t current_angle_PWM;

uint16_t	adcDMA[MAX_DATA];
uint16_t 	*ptr_adcTab;
uint8_t		koniec;
uint16_t 	tmp_var;

#endif /* GLOBAL_UTIL_H_ */
