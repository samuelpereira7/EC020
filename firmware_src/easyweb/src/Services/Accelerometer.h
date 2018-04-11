/*
 * Accelerometer.h
 *
 *  Created on: 10 de abr de 2018
 *      Author: samuelpereira
 */

#ifndef SERVICES_ACCELEROMETER_H_
#define SERVICES_ACCELEROMETER_H_

#include "acc.h"
#include <stdint.h>

void Accelerometer_init (void);
void Accelerometer_read (int8_t* x, int8_t* y, int8_t* z);
void Accelerometer_setRange(acc_range_t range);
void Accelerometer_setMode(acc_mode_t mode);


#endif /* SERVICES_ACCELEROMETER_H_ */
