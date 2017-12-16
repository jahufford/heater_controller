/*
 * debug_serial_port.h
 *
 *  Created on: Dec 6, 2017
 *      Author: joe
 */

#ifndef DEBUG_SERIAL_PORT_H_
#define DEBUG_SERIAL_PORT_H_

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef h_debug_uart;

uint8_t DebugSerialPort_Init();

#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


#ifdef __cplusplus
}
#endif

#endif /* DEBUG_SERIAL_PORT_H_ */
