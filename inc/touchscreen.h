/*
 * touchscreen.h
 *
 *  Created on: Dec 5, 2017
 *      Author: joe
 */
#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_

#include "stm32f4xx_hal.h"

#ifdef _cplusplus
extern "C" {
#endif

extern SPI_HandleTypeDef  h_touchscreen_spi;
extern uint8_t touchscreen_is_pressed;
extern TIM_HandleTypeDef h_touchpressed_ticker;

#define TOUCHSCREEN_SPI					 SPI2
#define TOUCHSCREEN_IRQ_PIN              GPIO_PIN_7
#define TOUCHSCREEN_IRQ_PORT             GPIOA
#define TOUCHSCREEN_SPI_SCK_PIN			 GPIO_PIN_7
#define TOUCHSCREEN_SPI_SCK_PORT		 GPIOC
#define TOUCHSCREEN_SPI_MOSI_PIN	     GPIO_PIN_3
#define TOUCHSCREEN_SPI_MOSI_PORT	     GPIOC
#define TOUCHSCREEN_SPI_MISO_PIN		 GPIO_PIN_2
#define TOUCHSCREEN_SPI_MISO_PORT        GPIOC
#define TOUCHSCREEN_SPI_CS_PIN           GPIO_PIN_7
#define TOUCHSCREEN_SPI_CS_PORT          GPIOB
#define TOUCHSCREEN_SPI_CLK_ENABLE()     __HAL_RCC_SPI2_CLK_ENABLE()

#define TOUCHSCREEN_CS_LOW() 	HAL_GPIO_WritePin(TOUCHSCREEN_SPI_CS_PORT,TOUCHSCREEN_SPI_CS_PIN,GPIO_PIN_RESET)
#define TOUCHSCREEN_CS_HIGH() 	HAL_GPIO_WritePin(TOUCHSCREEN_SPI_CS_PORT,TOUCHSCREEN_SPI_CS_PIN,GPIO_PIN_SET)

#define TOUCHSCREEN_RAW_MAX_Y			 3500
#define TOUCHSCREEN_RAW_MIN_Y			 260
#define TOUCHSCREEN_RAW_MAX_X			 3550
#define TOUCHSCREEN_RAW_MIN_X			 125

uint8_t TS_HardwareInit();
uint8_t TS_IsPressed();
void    TS_WriteData(uint8_t data);
int16_t TS_GetX(uint8_t num_samples_for_average);
int16_t TS_GetY(uint8_t num_samples_for_average);
void    TS_SetIdle();
void	TS_StartRead();
void	TS_EndRead();

#ifdef _cplusplus
}
#endif

#endif /* TOUCHSCREEN_H_ */
