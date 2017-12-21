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

#define TEMP_ERROR INT_MIN

uint8_t Temperature_HardwareInit(void);
int     Temperature_Read();

#ifdef __cplusplus
}
#endif

#endif /* TEMPERATURE_H_ */
