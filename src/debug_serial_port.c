/*
 * debug_serial_port.c
 *
 *  Created on: Dec 6, 2017
 *      Author: joe
 */

#include "debug_serial_port.h"
#include "stdlib.h"

UART_HandleTypeDef h_debug_uart;

/* Definition for USARTx Pins */
#define DEBUG_USART_TX_PIN                GPIO_PIN_2
#define DEBUG_USART_TX_GPIO_PORT          GPIOA
#define DEBUG_USART_TX_AF                 GPIO_AF7_USART2
#define DEBUG_USART_RX_PIN                GPIO_PIN_3
#define DEBUG_USART_RX_GPIO_PORT          GPIOA
#define DEBUG_USART_RX_AF                 GPIO_AF7_USART2

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  HAL_UART_Transmit(&h_debug_uart, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}

uint8_t DebugSerialPort_Init()
{

	__HAL_RCC_USART2_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	GPIO_InitTypeDef GPIO_InitStruct;
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = DEBUG_USART_TX_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = DEBUG_USART_TX_AF;

	HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);

	/* UART RX GPIO pin configuration  */
	GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
	GPIO_InitStruct.Alternate = DEBUG_USART_RX_AF;

	HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);

	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART1 configured as follows:
	    - Word Length = 8 Bits
	    - Stop Bit = One Stop bit
	    - Parity = ODD parity
	    - BaudRate = 9600 baud
	    - Hardware flow control disabled (RTS and CTS signals) */
	//UartHandle.Instance          = USARTx;
	h_debug_uart.Instance 		   = USART2;
	h_debug_uart.Init.BaudRate     = 9600;
	h_debug_uart.Init.WordLength   = UART_WORDLENGTH_8B;
	  //UartHandle.Init.WordLength   = UART_WORDLENGTH_9B;
	h_debug_uart.Init.StopBits     = UART_STOPBITS_1;
	  // to get parity to work right, you must use 9 bit transmission
	  //UartHandle.Init.Parity       = UART_PARITY_ODD;
	  //UartHandle.Init.Parity       = UART_PARITY_EVEN;
	h_debug_uart.Init.Parity       = UART_PARITY_NONE;
	h_debug_uart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	h_debug_uart.Init.Mode         = UART_MODE_TX_RX;
	h_debug_uart.Init.OverSampling = UART_OVERSAMPLING_16;
	if(HAL_UART_Init(&h_debug_uart) != HAL_OK)
	{
	  /* Initialization Error */
	  //Error_Handler();
		return 0;
	}
	printf("Starting up...\n\r");
	fflush(stdout);

	return 1;
}
