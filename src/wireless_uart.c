/*
 * wireless_uart.c
 *
 *  Created on: Dec 21, 2017
 *      Author: joe
 */


#include "wireless_uart.h"

UART_HandleTypeDef UartHandleWireless;
// return 1 on success, 0 on failure
uint8_t WirelessUart_HardwareInit(void)
{
	//TODO enable port clock, and make defines for everything
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	// configure UART to talk to wireless module
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	UartHandleWireless.Instance 		   = USART1;
	UartHandleWireless.Init.BaudRate     = 9600;
	UartHandleWireless.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandleWireless.Init.StopBits     = UART_STOPBITS_1;
	UartHandleWireless.Init.Parity       = UART_PARITY_NONE;
	UartHandleWireless.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandleWireless.Init.Mode         = UART_MODE_TX_RX;
	UartHandleWireless.Init.OverSampling = UART_OVERSAMPLING_16;
	if(HAL_UART_Init(&UartHandleWireless) != HAL_OK)
	{
	  /* Initialization Error */
	    Error_Handler();
	}

    USART1->CR1 |= (0x1 << 5); // enable receive interrupt
    NVIC_SetPriority(USART1_IRQn, 1);
    NVIC_ClearPendingIRQ(USART1_IRQn);
    NVIC_EnableIRQ(USART1_IRQn);

    return 1;
}
