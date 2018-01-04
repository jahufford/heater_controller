/*
 * globals.h
 *
 *  Created on: Dec 15, 2017
 *      Author: joe
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "error.h"
#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t temp_queue;
extern QueueHandle_t sent_queue;

#endif /* GLOBALS_H_ */
