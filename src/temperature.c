/*
 * temperature.c
 *
 *  Created on: Dec 20, 2017
 *      Author: joe
 */

#include "stm32f4xx.h"
#include "temperature.h"
/* Definition for USARTx clock resources */


TIM_HandleTypeDef  h_timer;
TIM_OC_InitTypeDef h_timer_oc_config;
ADC_HandleTypeDef  h_adc;
ADC_HandleTypeDef  h_gnd_adc;

// in order to support negative temperature, the LM35 temperature sensor
// needs to be able to output a negative voltage relative to its ground.
// so in order to do that, we've created a virtual ground, by using a diode
// to create a ~0.6 voltage reference that's connected to the sensor's ground.
// So negative temperatures are represented from 0 to 0.6V.
// The 5.4V is made by using a charge pump driven by a pwm.
//   _____ 5.4V            ___ 3.3V
//     |                    |
//    ---                   |
//    | |                   \
//    | |---- temp output   /
//    | |                   \
//    ---                   /
//     |                    |
//     ---------------------- 0.6V
//                         _|__
//                         \  /
//                          \/
//                         ----
//                          |
//                        ------
//                         ----
//                          --

// return 1 on success, 0 on failure
uint8_t Temperature_HardwareInit(void)
{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	    __HAL_RCC_GPIOA_CLK_ENABLE();
//	    __HAL_RCC_GPIOC_CLK_ENABLE();
//	    __HAL_RCC_GPIOB_CLK_ENABLE();
//	    // setup GPIO for PWM output
//	    GPIO_InitTypeDef GPIO_InitStruct2;
//	//    GPIO_InitStruct2.Pin = GPIO_PIN_15;
//	//    //GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	//    GPIO_InitStruct2.Mode = GPIO_MODE_AF_PP;
//	//   // GPIO_InitStruct.Pull = GPIO_NOPULL;
//	//    GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_HIGH;
//	//    GPIO_InitStruct2.Alternate = GPIO_AF9_TIM12;
//	//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct2);
//	    GPIO_InitStruct2.Pin = GPIO_PIN_15;
//	    GPIO_InitStruct2.Mode = GPIO_MODE_AF_PP;
//	    GPIO_InitStruct2.Pull = GPIO_NOPULL;
//	    //GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_HIGH;
//	    GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_LOW;
//	    GPIO_InitStruct2.Alternate = GPIO_AF9_TIM12;
//	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct2);
//
//	    //__HAL_RCC_TIM12_CLK_ENABLE();
//
//	ENABLE_CHARGEPUMP_PWM_TIMER_CLK();
//	    TIM_HandleTypeDef h_timer;
//	    TIM_OC_InitTypeDef h_timer_oc_config;
//	    h_timer.Instance = TIM12;
//	    h_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	    h_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
//	    //h_timer.Init.Period = 0xFFFF; // Max
//	    //h_timer.Init.Period = 0xFFFF;// Max
//	    h_timer.Init.Period = 1000;// Max
//	    h_timer.Init.Prescaler = 2;
//	    h_timer.State = HAL_TIM_STATE_RESET;
//	    //h_timer.Channel = HAL_TIM_ACTIVE_CHANNEL_2;
//
//	    h_timer_oc_config.OCFastMode = TIM_OCFAST_ENABLE;
//	    h_timer_oc_config.OCMode = TIM_OCMODE_PWM1;
//	    h_timer_oc_config.OCNIdleState = TIM_OCNIDLESTATE_SET;
//	    //h_timer_oc_config.OCNPolarity = TIM_OCNPOLARITY_LOW;
//	    h_timer_oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
//	    //h_timer_oc_config.Pulse = 0xFFFF/2;
//	    h_timer_oc_config.Pulse = 500;
//	    HAL_TIM_Base_Init(&h_timer);
//	    HAL_TIM_PWM_Init(&h_timer);
//	    //HAL_TIM_Base_Start(&h_timer);
//		HAL_TIM_PWM_ConfigChannel(&h_timer, &h_timer_oc_config,TIM_CHANNEL_2);
//	    HAL_TIM_PWM_Start(&h_timer,TIM_CHANNEL_2);


//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOC_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
	// setup GPIO for PWM output
    ENABLE_TEMP_PWM_PORT_CLK(); // clock must be enabled before call to HAL_GPIO_INIT
    GPIO_InitTypeDef GPIO_InitStruct2;
    GPIO_InitStruct2.Pin = TEMP_PWM_PIN;
    GPIO_InitStruct2.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct2.Pull = GPIO_NOPULL;
    GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct2.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(TEMP_PWM_PORT, &GPIO_InitStruct2);

    // config the timer hardware that the charge pump uses
	ENABLE_CHARGEPUMP_PWM_TIMER_CLK(); // clock must b enabled before call to HAL_TIM_PWM_INIT()
    h_timer.Instance = CHARGEPUMP_TIMER;
    h_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    h_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    h_timer.Init.Period = 1000;// Max
    h_timer.Init.Prescaler = 2;
    h_timer.State = HAL_TIM_STATE_RESET;

    h_timer_oc_config.OCFastMode = TIM_OCFAST_ENABLE;
    h_timer_oc_config.OCMode = TIM_OCMODE_PWM1;
    h_timer_oc_config.OCNIdleState = TIM_OCNIDLESTATE_SET;
    h_timer_oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
    h_timer_oc_config.Pulse = 500;
    HAL_TIM_Base_Init(&h_timer);
    HAL_TIM_PWM_Init(&h_timer);
	HAL_TIM_PWM_ConfigChannel(&h_timer, &h_timer_oc_config,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&h_timer,TIM_CHANNEL_2);

    // set up a-to-d analog pin
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = TEMP_ADC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TEMP_ADC_PORT, &GPIO_InitStruct);
    ENABLE_TEMP_ADC_PORT_CLK();

    // main adc for reading the temperature
    ENABLE_TEMP_ADC_CLK();
    h_adc.Instance = TEMP_ADC;
    h_adc.State = HAL_ADC_STATE_RESET;
    h_adc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // set to value, but it's not used in async mode
    h_adc.Init.Resolution = ADC_RESOLUTION_12B;
    h_adc.Init.ContinuousConvMode = DISABLE;
    h_adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    h_adc.Init.ScanConvMode = DISABLE;
    h_adc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    h_adc.Init.NbrOfConversion = 1;
    h_adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    h_adc.Init.DMAContinuousRequests = DISABLE;
    h_adc.Init.NbrOfDiscConversion = 1;
    //h_adc.Init.
    HAL_ADC_Init(&h_adc);
    ADC_ChannelConfTypeDef adc_config;
    adc_config.Channel = ADC_CHANNEL_1;
    adc_config.Rank = 1;
    adc_config.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    HAL_ADC_ConfigChannel(&h_adc, &adc_config);


    // adc to read the temp sensor's virtual ground
    GPIO_InitStruct.Pin = TEMP_VIRTUAL_GND_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TEMP_VIRTUAL_GND_PORT, &GPIO_InitStruct);
    ENABLE_VIRTUAL_GND_ADC_PORT_CLK();

    ENABLE_VIRTUAL_GND_ADC_CLK();
    h_gnd_adc.Instance = VIRTUAL_GND_ADC;
    h_gnd_adc.State = HAL_ADC_STATE_RESET;
    h_gnd_adc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // set to value, but it's not used in async mode
    h_gnd_adc.Init.Resolution = ADC_RESOLUTION_12B;
    h_gnd_adc.Init.ContinuousConvMode = DISABLE;
    h_gnd_adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    h_gnd_adc.Init.ScanConvMode = DISABLE;
    h_gnd_adc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    h_gnd_adc.Init.NbrOfConversion = 1;
    h_gnd_adc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    h_gnd_adc.Init.DMAContinuousRequests = DISABLE;
    h_gnd_adc.Init.NbrOfDiscConversion = 1;

    if(HAL_ADC_Init(&h_gnd_adc) != HAL_OK){
    	return 0;
    }

    ADC_ChannelConfTypeDef gnd_adc_config;
    //gnd_adc_config.Channel = ADC_CHANNEL_4;
    gnd_adc_config.Channel =ADC_CHANNEL_8;
    gnd_adc_config.Rank = 1;
    gnd_adc_config.SamplingTime = ADC_SAMPLETIME_480CYCLES;

    if(HAL_ADC_ConfigChannel(&h_gnd_adc, &gnd_adc_config) != HAL_OK){
    	return 0;
    }
    return 1;
}

// return temperature in farenheit, return TEMP_ERROR on error
int Temperature_Read()
{
    volatile uint32_t temp_sum = 0;
    volatile uint32_t gnd_sum = 0;
	volatile uint32_t temp_min = 0xFFFF;
	volatile uint32_t temp_min2 = 0xFFFF;
	volatile uint32_t temp_max = 0;
	volatile uint32_t temp_max2 = 0;
    volatile uint32_t result;
    volatile uint32_t val;

    // read temperature several times, throw out 2 highest and 2 lowest, average the rest
    // we're using 12 bit resolution on the adc
    temp_sum = 0;
	for(int i=0;i<30;i++){
        if(HAL_ADC_Start(&h_adc)!= HAL_OK){
        	return TEMP_ERROR;
        }
        result = HAL_ADC_PollForConversion(&h_adc,100);
        if(result != HAL_OK)
        {
        	return TEMP_ERROR;
        }
        val = HAL_ADC_GetValue(&h_adc);
        if(val>=temp_max){
           temp_max2 = temp_max;
           temp_max = val;
        }
        if(val>=temp_max2){
           temp_max2 = val;
        }
        if(val<=temp_min){
           temp_min2 = temp_min;
           temp_min = val;
        }
        if(val<=temp_min2){
           temp_min2 = val;
        }
        temp_sum+=val;
	}
	// throw away the 2 highest and 2 lowest values
	temp_sum -= temp_max;
	temp_sum -= temp_max2;
	temp_sum -= temp_min;
	temp_sum -= temp_min2;
	// average the rest of the rest of the samples
	volatile uint32_t temp_avg = temp_sum/26;

	gnd_sum =0;
    for(int i=0;i<30;i++){
        if(HAL_ADC_Start(&h_gnd_adc) != HAL_OK){
        	return TEMP_ERROR;
        }
        if(HAL_ADC_PollForConversion(&h_gnd_adc,100) != HAL_OK){
        	return TEMP_ERROR;
        }
        gnd_sum += HAL_ADC_GetValue(&h_gnd_adc);
    }
	volatile uint32_t gnd_avg;
    gnd_avg = gnd_sum/30;

    // subtract the virtual ground value from the temperature value
	int temp_diff = temp_avg - gnd_avg;
    int temp = (int)((temp_diff/12.412)*9)/5 + 32;
//    int temp = (int)((temp_diff/7.585)*9)/5 + 32;
    return temp;
}
