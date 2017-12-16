/*
 * touchscreen.c
 *
 *  Created on: Dec 5, 2017
 *      Author: joe
 */

#include "touchscreen.h"
#include "lcd_hardware.h"

SPI_HandleTypeDef  h_touchscreen_spi;

uint8_t touchscreen_is_pressed;
TIM_HandleTypeDef h_touchpressed_ticker;

// returns 1 on success, 0 on failure
uint8_t TS_HardwareInit()
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	//__HAL_RCC_GPIOF_CLK_ENABLE();
    // set up Touchscreen SPI ports and pins
    // Touchscreen SCK
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = TOUCHSCREEN_SPI_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(TOUCHSCREEN_SPI_SCK_PORT, &GPIO_InitStruct);

    // SPI MISO
    GPIO_InitStruct.Pin = TOUCHSCREEN_SPI_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(TOUCHSCREEN_SPI_MISO_PORT, &GPIO_InitStruct);

    // SPI MOSI
    GPIO_InitStruct.Pin = TOUCHSCREEN_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(TOUCHSCREEN_SPI_MOSI_PORT, &GPIO_InitStruct);

    // SPI chip select
    GPIO_InitStruct.Pin = TOUCHSCREEN_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(TOUCHSCREEN_SPI_CS_PORT, &GPIO_InitStruct);
   	HAL_GPIO_WritePin(TOUCHSCREEN_SPI_CS_PORT,TOUCHSCREEN_SPI_CS_PIN,GPIO_PIN_SET);
   	HAL_GPIO_WritePin(TOUCHSCREEN_SPI_CS_PORT,TOUCHSCREEN_SPI_CS_PIN,GPIO_PIN_RESET);
   	HAL_GPIO_WritePin(TOUCHSCREEN_SPI_CS_PORT,TOUCHSCREEN_SPI_CS_PIN,GPIO_PIN_SET);

   	// set up touchscreen IRQ external input pin
    GPIO_InitStruct.Pin = TOUCHSCREEN_IRQ_PIN;
   	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
   	GPIO_InitStruct.Pull = GPIO_NOPULL;
   	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   	HAL_GPIO_Init(TOUCHSCREEN_IRQ_PORT, &GPIO_InitStruct);

   	uint32_t reg;
    reg = SYSCFG->EXTICR[1];
    reg &= ~(SYSCFG_EXTICR2_EXTI7_Msk);  // ~(0xF << 12)
    reg |= (SYSCFG_EXTICR2_EXTI7_PA);
    SYSCFG->EXTICR[1] = reg;
    touchscreen_is_pressed = 0;
    __HAL_GPIO_EXTI_CLEAR_IT(TOUCHSCREEN_IRQ_PIN);
    NVIC_SetPriority(EXTI9_5_IRQn, 1);
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    TOUCHSCREEN_SPI_CLK_ENABLE();

    h_touchscreen_spi.Instance = TOUCHSCREEN_SPI;
    //h_lcd_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    h_touchscreen_spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    h_touchscreen_spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    h_touchscreen_spi.Init.CLKPolarity = SPI_POLARITY_LOW; // might need to be high with 2nd edge
    h_touchscreen_spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    h_touchscreen_spi.Init.CRCPolynomial;
    h_touchscreen_spi.Init.DataSize = SPI_DATASIZE_8BIT;
    h_touchscreen_spi.Init.Direction = SPI_DIRECTION_2LINES;
    h_touchscreen_spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    h_touchscreen_spi.Init.Mode = SPI_MODE_MASTER;
    //h_touchscreen_spi.Init.NSS = SPI_NSS_HARD_OUTPUT;
    h_touchscreen_spi.Init.NSS = SPI_NSS_SOFT;
    //h_lcd_spi.Init.NSS = SPI_NSS_SOFT;
    h_touchscreen_spi.Init.TIMode = SPI_TIMODE_DISABLE;
    h_touchscreen_spi.State = HAL_SPI_STATE_RESET;

    if(HAL_SPI_Init(&h_touchscreen_spi) != HAL_OK){
    	return 0;
    }

    // set up movement timer and interrupt
    __HAL_RCC_TIM5_CLK_ENABLE();
    // set up a basic ticker
    h_touchpressed_ticker.Instance = TIM5;
    h_touchpressed_ticker.State = HAL_TIM_STATE_RESET;
    h_touchpressed_ticker.Init.ClockDivision = 0;
    h_touchpressed_ticker.Init.CounterMode = TIM_COUNTERMODE_UP;
    h_touchpressed_ticker.Init.Period = 9000*10;
    h_touchpressed_ticker.Init.Prescaler = 0;
    HAL_TIM_Base_Init(&h_touchpressed_ticker);
    h_touchpressed_ticker.Instance->DIER |= 1;
	TIM5->SR &= ~TIM_SR_UIF;
//	NVIC_SetPriority(TIM5_IRQn, 1);
//    NVIC_ClearPendingIRQ(TIM5_IRQn);
//    NVIC_EnableIRQ(TIM5_IRQn);

	return 1;
}

uint8_t TS_IsPressed()
{
	return touchscreen_is_pressed;
}

void TS_WriteData(uint8_t data) {
	if(HAL_SPI_Transmit(&h_touchscreen_spi,(uint8_t*)&data, 1,0xFFFF) != HAL_OK){
		Error_Handler();
    }
}

// returns negative number on error
int16_t TS_GetX(uint8_t num_samples_for_average)
{
	uint32_t sum=0;
    uint8_t bytes[2];
    EXTI->IMR &= ~(TOUCHSCREEN_IRQ_PIN); // mask the interrupt because during reads the irq moves around
    // read x position, actually it's the y for the TS, but we've rotated to landscape mode
    for(int i=0;i<num_samples_for_average;i++){
        TOUCHSCREEN_CS_LOW();
        TS_WriteData(0x99);
        if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)bytes, 1,0xFFFF) != HAL_OK){
           Error_Handler();
        }
        if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)&bytes[1], 1,0xFFFF) != HAL_OK){
           Error_Handler();
        }
        TOUCHSCREEN_CS_HIGH();
        uint16_t datax = bytes[0];
        datax <<= 8;
        datax |= bytes[1];
        datax >>= 3;
        sum += datax;
    }
    EXTI->IMR |= TOUCHSCREEN_IRQ_PIN; // unmask the interrupt
    uint16_t avg = sum/num_samples_for_average;
    // convert to pixel coordinate
    uint32_t pixel = (avg-TOUCHSCREEN_RAW_MIN_X);
    pixel = (pixel << 8) + (pixel << 6); // multiply pixel by 320
    pixel /= TOUCHSCREEN_RAW_MAX_X;
    // if you're pressing outside the valid area
    pixel = pixel>=LCD_XSIZE ? -1 : pixel;
    return (int16_t)pixel;
}

// returns negative number on error
int16_t TS_GetY(uint8_t num_samples_for_average)
{
	uint32_t sum=0;
    uint8_t bytes[2];
    EXTI->IMR &= ~(TOUCHSCREEN_IRQ_PIN); // mask the interrupt because during reads the irq moves around
    // read x position, actually it's the y for the TS, but we've rotated to landscape mode
    for(int i=0;i<num_samples_for_average;i++){
        TOUCHSCREEN_CS_LOW();
        TS_WriteData(0xD9);
        if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)bytes, 1,0xFFFF) != HAL_OK){
           Error_Handler();
        }
        if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)&bytes[1], 1,0xFFFF) != HAL_OK){
           Error_Handler();
        }
        TOUCHSCREEN_CS_HIGH();
        uint16_t datax = bytes[0];
        datax <<= 8;
        datax |= bytes[1];
        datax >>= 3;
        sum += datax;
    }
    EXTI->IMR |= TOUCHSCREEN_IRQ_PIN; // unmask the interrupt
    uint16_t avg = sum/num_samples_for_average;
    // convert to pixel coordinate
    uint32_t pixel = (avg-TOUCHSCREEN_RAW_MIN_Y);
    pixel = (pixel << 8) - (pixel << 4); // multiply pixel by 240
    pixel /= TOUCHSCREEN_RAW_MAX_Y;
    // if you're pressing outside the valid area
    pixel = pixel>=LCD_YSIZE ? -1 : pixel;
    return (int16_t)pixel;
}

void TS_SetIdle()
{
	uint8_t bytes[2];
    EXTI->IMR &= ~(TOUCHSCREEN_IRQ_PIN); // mask the interrupt because during reads the irq moves around
	TOUCHSCREEN_CS_LOW();
    //TS_WriteData(0x90); // back to idle
    TS_WriteData(0xD0); // back to idle
    if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)bytes, 1,0xFFFF) != HAL_OK){
    	Error_Handler();
    }
    if(HAL_SPI_Receive(&h_touchscreen_spi,(uint8_t*)&bytes[1], 1,0xFFFF) != HAL_OK){
        Error_Handler();
    }
    // one nop makes a delay of about 100ns from when the final clock goes low to
    // when the CS line goes high
    // but 8 nop's is 200ns
    // but for(int i=0;i<1;i++){asm("nop");} takes 400ns, go figure. Be a good spot to test
    // if compilers optimizations makes them equivalent
    // this bit of a delay is needed to keep the PENIRQ line of the touchscreen functioning
    // no idea why this is needed, but otherwise the irq line quits working
    for(int i=0;i<1;i++){
    	asm("nop");
    }
    EXTI->IMR |= TOUCHSCREEN_IRQ_PIN; // unmask the interrupt
    TOUCHSCREEN_CS_HIGH();
}

void TS_StartRead()
{
	// this keeps the IRQ pin low while reading from the touchscreen controller
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = TOUCHSCREEN_IRQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TOUCHSCREEN_IRQ_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(TOUCHSCREEN_IRQ_PORT,TOUCHSCREEN_IRQ_PIN,GPIO_PIN_RESET);
}

void TS_EndRead()
{
	// let the IRQ pin act as a rising edge interrupt line to detect
	// when a press if lifted
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = TOUCHSCREEN_IRQ_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TOUCHSCREEN_IRQ_PORT, &GPIO_InitStruct);
}
