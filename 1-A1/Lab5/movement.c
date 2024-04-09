/*
 * movement.c
 *
 *  Created on: Jan 30, 2024
 *      Author: cihem
 */
#include "movement.h"

double moveForward(oi_t *sensor_data, double distance_mm) {
    double sum = 0; // distance member in oi_t struct is type double

    lcd_printf("%.2lf", sum);

    oi_setWheels(500,500); //move forward at full speed

    while (sum < distance_mm) {
        if(sensor_data->bumpLeft) {
            oi_setWheels(0,0); //stop
            moveBack(sensor_data, 150);
            turnRight(sensor_data, 90);
            moveForward(sensor_data, 250);
            turnLeft(sensor_data, 90);
            oi_setWheels(500,500);
        } else if(sensor_data->bumpRight) {
            oi_setWheels(0,0); //stop
            moveBack(sensor_data, 150);
            turnLeft(sensor_data, 90);
            moveForward(sensor_data, 250);
            turnRight(sensor_data, 90);
            oi_setWheels(500,500);
        } else {
            oi_update(sensor_data);
            sum += sensor_data -> distance; // use -> notation since pointer
            lcd_clear();
            lcd_printf("%.2lf", sum);
        }
    }
    oi_setWheels(0,0); //stop

    return sum;
}

void turnRight(oi_t *sensor, double degrees) {
    double angle = 0;

    oi_setWheels(-200, 200);

    while(angle > degrees*-1) {
        oi_update(sensor);
        angle += sensor->angle;
    }

    oi_setWheels(0, 0);
}

void turnLeft(oi_t *sensor, double degrees) {
    double angle = 0;

    oi_setWheels(200, -200);

    while(angle < degrees) {
        oi_update(sensor);
        angle += sensor->angle;
    }

    oi_setWheels(0, 0);
}

double moveBack(oi_t *sensor_data, double distance_mm) {
    double sum = 0; // distance member in oi_t struct is type double

    lcd_printf("%.2lf", sum);

    oi_setWheels(-200,-200); //move forward at full speed

    while (sum > distance_mm*-1) {
        oi_update(sensor_data);
        sum += sensor_data -> distance; // use -> notation since pointer
        lcd_clear();
        lcd_printf("%.2lf", sum);
    }
    oi_setWheels(0,0); //stop

    return sum;
}

void bump(oi_t *sensor) {
    if(sensor->lightBumpLeftSignal > 100 || sensor->lightBumpCenterRightSignal > 100) {
        moveBack(sensor, 150);
    }
}






