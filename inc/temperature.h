/*
 * temperature.h
 *
 *  Created on: Dec 20, 2017
 *      Author: joe
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEMP_ERROR            INT_MIN

#define CHARGE_PUMP_TIMER                TIM12
#define TEMP_ADC                         ADC1
#define VIRTUAL_GND_ADC                  ADC2

#define ENABLE_TEMP_ADC_CLK()            __HAL_RCC_ADC1_CLK_ENABLE()
#define ENABLE_VIRTUAL_GND_ADC_CLK()     __HAL_RCC_ADC2_CLK_ENABLE()
#define ENABLE_CHARGE_PUMP_PWM_TIMER()   __HAL_RCC_TIM12_CLK_ENABLE()


#define TEMP_PWM_PIN          GPIO_PIN_15
#define TEMP_PWM_PORT         GPIOB
#define ENABLE_TEMP_PWM_PORT()           __HAL_RCC_GPIOB_CLK_ENABLE()

#define TEMP_ADC_PIN          GPIO_PIN_1
#define TEMP_ADC_PORT         GPIOA
#define ENABLE_TEMP_ADC_PORT_CLK()       __HAL_RCC_GPIOA_CLK_ENABLE()

#define TEMP_VIRTUAL_GND_PIN  GPIO_PIN_0
#define TEMP_VIRTUAL_GND_PORT GPIOB
#define ENABLE_VIRTUAL_GND_ADC_PORT()    __HAL_RCC_GPIOB_CLK_ENABLE()

uint8_t Temperature_HardwareInit(void);
int     Temperature_Read();

#ifdef __cplusplus
}
#endif

#endif /* TEMPERATURE_H_ */
