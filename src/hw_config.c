/*
 *  hw_config.c
 *
 *  Created on: 26 lut 2016
 *      Author: Marek Frydrysiak
 */

#include "stm32f10x.h"
#include "global_util.h"

void GPIO_Configuration();
void UART_Configuration();
void DMA_Configuration();
void TIM_Configuration();
void ADC_Configuration();
void NVIC_Configuration();

void hardware_config()
{
	SysTick_Config(SystemCoreClock / 1000);  //1ms przy delay(1)
	GPIO_Configuration();
	UART_Configuration();
	DMA_Configuration();
	ADC_Configuration();
	TIM_Configuration();
	NVIC_Configuration();
}

void GPIO_Configuration()
{
	GPIO_InitTypeDef gpio_struct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// Alternative functions enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOD, ENABLE);	// GPIOx clock enable

	// Configure GPIOs
	/* PWM Pin configuration */
	gpio_struct.GPIO_Pin = GPIO_Pin_8;
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpio_struct);

	/* ADC Pin configuration */
	gpio_struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_struct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &gpio_struct);

	/* Sensor - pulse generator */
	/* Sensor 1 - LEFT */
	gpio_struct.GPIO_Pin = GPIO_Pin_9;
	gpio_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &gpio_struct);
	/* Sensor 2 - RIGHT */
	gpio_struct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &gpio_struct);

	/* UART (RX, TX) Pins configuration */
	gpio_struct.GPIO_Pin = GPIO_Pin_2;
	gpio_struct.GPIO_Mode = GPIO_Mode_AF_PP;
	//gpio_struct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &gpio_struct); //TX
	gpio_struct.GPIO_Pin = GPIO_Pin_3;
	gpio_struct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio_struct);	//RX
}

void UART_Configuration()
{
	USART_InitTypeDef uart_struct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	// USART2 enable;

	// Configure UART
	USART_StructInit(&uart_struct);
	uart_struct.USART_BaudRate = 115200;
	USART_Init(USART2, &uart_struct);

	USART_Cmd(USART2, ENABLE);	// Start UART
}

void DMA_Configuration()
{
	DMA_InitTypeDef dma_struct;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_StructInit(&dma_struct);

	dma_struct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	dma_struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	dma_struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	dma_struct.DMA_MemoryBaseAddr = (uint32_t)adc_value;
	dma_struct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	dma_struct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	dma_struct.DMA_DIR = DMA_DIR_PeripheralSRC;
	dma_struct.DMA_BufferSize = 2;						// number of channels
	dma_struct.DMA_Mode = DMA_Mode_Circular;
	DMA_Init(DMA1_Channel1, &dma_struct);

	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void ADC_Configuration()
{
	ADC_InitTypeDef adc_struct;

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_StructInit(&adc_struct);
	adc_struct.ADC_ScanConvMode = ENABLE;
	adc_struct.ADC_ContinuousConvMode = ENABLE;
	adc_struct.ADC_NbrOfChannel = 2;
	adc_struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init(ADC1, &adc_struct);

	//ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_1Cycles5);

	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void TIM_Configuration()
{
	TIM_TimeBaseInitTypeDef		tim_struct;
	TIM_OCInitTypeDef			channel_struct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |
						   RCC_APB1Periph_TIM4, ENABLE); 	// TIM2, TIM4 clock enable

	/* TIM2 */
	/* Configure TIM2 (For ADC reading - DMA access) */

	TIM_TimeBaseStructInit(&tim_struct);
	tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
	tim_struct.TIM_Prescaler = 64 - 1;
	//tim_struct.TIM_Period = 100 - 1;	//0,1ms
	tim_struct.TIM_Period = 10 - 1;		//20us
	TIM_TimeBaseInit(TIM2, &tim_struct);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	/* TIM4 */
	/* Configure TIM4 (PWM for servomechanism) */
	tim_struct.TIM_Period = PWM_PERIOD_20MS;
	tim_struct.TIM_Prescaler = PWM_TIM_PRESCALER;
	tim_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &tim_struct);

	channel_struct.TIM_OCMode = TIM_OCMode_PWM1;
	channel_struct.TIM_Pulse = PWM_INITIAL_POS;					// 50%
	channel_struct.TIM_OutputState = TIM_OutputState_Enable;
	channel_struct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &channel_struct);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);

	TIM_Cmd(TIM4, ENABLE);		// Start TIM
}

void NVIC_Configuration()
{
	NVIC_InitTypeDef nvic_struct;

	nvic_struct.NVIC_IRQChannel = TIM2_IRQn;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_struct.NVIC_IRQChannelSubPriority = 0;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_struct);
}
