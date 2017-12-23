/*
 * wireless_uart.h
 *
 *  Created on: Dec 21, 2017
 *      Author: joe
 */

#ifndef WIRELESS_UART_H_
#define WIRELESS_UART_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "queue.h"

extern UART_HandleTypeDef UartHandleWireless;
extern QueueHandle_t wireless_queue;

uint8_t WirelessUart_HardwareInit(void);



#ifdef __cplusplus
}
#endif



#endif /* WIRELESS_UART_H_ */
