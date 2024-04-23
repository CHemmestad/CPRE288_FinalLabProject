/*
 * BorderDetect.h
 *
 *  Created on: Apr 23, 2024
 *      Author: kadenb
 */

#ifndef BORDERDETECT_H_
#define BORDERDETECT_H_



#include "open_interface.h"
#include <stdint.h>

uint32_t borderDetected(oi_t *sensor_data);
void moveForwardWithBorderDetect(oi_t *sensor_data, double distance_mm);

#endif /* BORDERDETECT_H_ */
