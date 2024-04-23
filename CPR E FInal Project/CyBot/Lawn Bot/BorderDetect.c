/*
 * BorderDetect.c
 *
 *  Created on: Apr 23, 2024
 *      Author: kadenb
 */

#include "open_interface.h"
#include <stdint.h>

uint32_t borderDetected(oi_t *sensor_data){

    //change from cliffFrontLeftSignal to the other cliff signals for different sensors
    return sensor_data -> cliffFrontLeftSignal;

}

void moveForwardWithBorderDetect(oi_t *sensor_data, double distance_mm){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(250,250); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);
        if(sensor_data -> cliffFrontLeftSignal > 2600 || sensor_data -> cliffFrontRightSignal > 2600){
            oi_setWheels(0,0);
            return;
        }

        sum += sensor_data -> distance; // use -> notation since pointer
        sensor_data -> distance = 0;
    }

    oi_setWheels(0,0);
    return;
}


