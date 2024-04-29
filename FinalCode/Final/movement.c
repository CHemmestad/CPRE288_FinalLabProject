/*
 * movement.c
 *
 *  Created on: Jan 30, 2024
 *      Author: cihem
 */
#include "movement.h"
#include "uart.h"

double moveForward(oi_t *sensor_data, double distance_mm) {
    double sum = 0; // distance member in oi_t struct is type double

    lcd_printf("%.2lf", sum);

    oi_setWheels(250,250); //move forward at full speed

    while (sum < distance_mm) {
//        if(sensor_data->bumpLeft) {
//            oi_setWheels(0,0); //stop
//            moveBack(sensor_data, 150);
//            turnRight(sensor_data, 90);
//            moveForward(sensor_data, 250);
//            turnLeft(sensor_data, 90);
//            oi_setWheels(500,500);
//        } else if(sensor_data->bumpRight) {
//            oi_setWheels(0,0); //stop
//            moveBack(sensor_data, 150);
//            turnLeft(sensor_data, 90);
//            moveForward(sensor_data, 250);
//            turnRight(sensor_data, 90);
//            oi_setWheels(500,500);
//        } else {
            oi_update(sensor_data);
            sum += sensor_data->distance; // use -> notation since pointer
            sensor_data->distance = 0;
            lcd_clear();
            lcd_printf("%.2lf", sum);
//        }
    }
    oi_setWheels(0,0); //stop

    return sum;
}

double turnRight(oi_t *sensor, double degrees) {
    double angle = 0;

    oi_setWheels(-200, 200);

    while(angle > degrees*-1) {
        oi_update(sensor);
        angle += sensor->angle;
    }

    oi_setWheels(0, 0);
    return angle;
}

double turnLeft(oi_t *sensor, double degrees) {
    double angle = 0;

    oi_setWheels(200, -200);

    while(angle < degrees) {
        oi_update(sensor);
        angle += sensor->angle;
    }

    oi_setWheels(0, 0);
    return angle;
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

double moveForwardWithDetect(oi_t *sensor_data, double distance_mm){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(250,250); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);

        if(sensor_data -> cliffLeftSignal > 2600) {
            oi_setWheels(0,0);
            char send = '0';
            uart_sendChar(send);
            return sum;
        } else if(sensor_data -> cliffFrontLeftSignal > 2600) {
            oi_setWheels(0,0);
            char send = '1';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> cliffFrontRightSignal > 2600) {
            oi_setWheels(0,0);
            char send = '2';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> cliffRightSignal > 2600) {
            oi_setWheels(0,0);
            char send = '3';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> cliffLeftSignal < 100) {
            oi_setWheels(0,0);
            char send = '4';
            uart_sendChar(send);
            return sum;
        } else if(sensor_data -> cliffFrontLeftSignal < 100) {
            oi_setWheels(0,0);
            char send = '5';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> cliffFrontRightSignal < 100) {
            oi_setWheels(0,0);
            char send = '6';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> cliffRightSignal < 100) {
            oi_setWheels(0,0);
            char send = '7';
            uart_sendChar(send);
            return sum;
        } if(sensor_data -> bumpLeft && sensor_data -> bumpRight) {
            oi_setWheels(0,0);
            char send = '8';
            uart_sendChar(send);
            return sum;
        } else if(sensor_data -> bumpLeft) {
            oi_setWheels(0,0);
            char send = '9';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> bumpRight) {
            oi_setWheels(0,0);
            char send = 'a';
            uart_sendChar(send);
            return sum;
        }

        sum += sensor_data -> distance; // use -> notation since pointer
        sensor_data -> distance = 0;
    }
    char send = ' ';
    uart_sendChar(send);
    oi_setWheels(0,0);
    return sum;
}







