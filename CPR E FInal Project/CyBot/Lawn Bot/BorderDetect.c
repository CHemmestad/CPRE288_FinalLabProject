/*
 * BorderDetect.c
 *
 *  Created on: Apr 23, 2024
 *      Author: kadenb
 */

#include "open_interface.h"
#include <stdint.h>

uint32_t borderDetected(oi_t *sensor_data){

    return sensor_data -> cliffFrontLeftSignal;

}


