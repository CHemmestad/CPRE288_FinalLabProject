/*
 * movement.c
 *
 *  Created on: Jan 30, 2024
 *      Author: kadenb
 */
#include "cyBot_Scan.h"
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "float.h"
#include "math.h"
#include "uart.h"
#include "BorderDetect.h"

#define VALUES_SIZE 90

typedef struct {
    int start_angle;
    int closest_angle;
    float closest_distance;
    int radial_width;
    float linear_width;
    int ending_angle;
}Object;

double move_forward(oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor_data, double degrees);
double turn_left(oi_t *sensor_data, double degrees);
double move_backward(oi_t *sensor_data, double distance_mm);
void getMessage(char message);
void findObjects();
void scanStuff();
void driveToSmallest(oi_t *sensor_data);
void scanMedianDist(int startAngle, int endingAngle, int objNum);
double move_forward_no_bump(oi_t *sensor_data, double distance_mm);
void bump_left(oi_t *sensor_data);
void bump_right(oi_t *sensor_data);
void moveForwardWithBorderDetect(oi_t *sensor_data, double distance_mm);
double moveBackwardBorder(oi_t *sensor_data, double distance_mm);
void turnRightBorder(oi_t *sensor_data, double degrees);
void turnLeftBorder(oi_t *sensor_data, double degrees);
void turnAroundLeft(oi_t *sensor_data);
void turnAroundRight(oi_t *sensor_data);
void moveForwardAndScan(cyBOT_Scan_t *scan, oi_t *sensor_data, double distance_mm);


int main(void) {

    //Initializing code
    timer_init();
    lcd_init();
    uart_init();
    cyBOT_init_Scan(0b0111);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    cyBOT_Scan_t scan;
    //cyBOT_SERVO_cal();
    /*
     * 280000 and 1256500 for cybot 5 in tla, need to recalibrate for upstairs lab
     */


    right_calibration_value = 274750;
    left_calibration_value = 1293250;

    //uint32_t cliffSignalValue;

//        while(1){
//
//
//        oi_update(sensor_data);
//        char result[50];
//        cliffSignalValue = borderDetected(sensor_data);
//        sprintf(result, "%d", cliffSignalValue);
//        lcd_printf(result);
//        timer_waitMillis(400);
//        lcd_clear();
//        }

   moveForwardAndScan(&scan, sensor_data, 300);

    //moveForwardWithBorderDetect(sensor_data, 10000);


    return 0;
 }

void moveForwardAndScan(cyBOT_Scan_t *scan, oi_t *sensor_data, double distance_mm){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(250,250); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);
        cyBOT_Scan(90, scan);
        if(scan->sound_dist < 15){
            oi_setWheels(0, 0);
            return;
        }
        sum += sensor_data -> distance; // use -> notation since pointer
        sensor_data -> distance = 0;
    }

    oi_setWheels(0,0);
    return;
}

void turnAroundLeft(oi_t *sensor_data){
    turnLeftBorder(sensor_data, 85);
    move_forward_no_bump(sensor_data, 190);
    turnLeftBorder(sensor_data, 85);
}

void turnLeftBorder(oi_t *sensor_data, double degrees) {
   double startingAngle = sensor_data -> angle;
   double currentAngle = startingAngle;

   oi_setWheels(100,-100);

   while(currentAngle < (startingAngle + degrees)) {
       oi_update(sensor_data);
       currentAngle += sensor_data -> angle;
   }
   oi_setWheels(0,0);
    return;
}

void turnRightBorder(oi_t *sensor_data, double degrees) {
    double startingAngle = sensor_data -> angle;
       double currentAngle = startingAngle;

       oi_setWheels(-100,100);

       while(currentAngle > (startingAngle - degrees)) {
           oi_update(sensor_data);
           currentAngle += sensor_data -> angle;
       }
       oi_setWheels(0,0);
        return;
}


double moveBackwardBorder(oi_t *sensor_data, double distance_mm){

    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-100,-100); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum -= sensor_data -> distance; // use -> notation since pointer
        lcd_init();
    }

    oi_setWheels(0,0);
    return sum;
}

double moveForwardWithDetect(oi_t *sensor_data, double distance_mm){
    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(250,250); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);

        if(sensor_data -> cliffLeftSignal > 2400) {
            oi_setWheels(0,0);
            char send = '0';
            uart_sendChar(send);
            return sum;
        } else if(sensor_data -> cliffFrontLeftSignal > 2400) {
            oi_setWheels(0,0);
            char send = '1';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> cliffFrontRightSignal > 2400) {
            oi_setWheels(0,0);
            char send = '2';
            uart_sendChar(send);
            return sum;
        }  else if(sensor_data -> cliffRightSignal > 2400) {
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

    oi_setWheels(0,0);
    return;
}


void getMessage(char message){
    char messageTwo[] = "Got a message: ";
    char nextline[] = "\r\n";

        uart_sendStr(messageTwo);
        uart_sendChar(message);
        uart_sendStr(nextline);
}

double move_forward_no_bump(oi_t *sensor_data, double distance_mm){
    double sum = 0; // distance member in oi_t struct is type double
        oi_setWheels(250,250); //move forward at full speed

        while (sum < distance_mm) {
            oi_update(sensor_data);
            sum += sensor_data -> distance; // use -> notation since pointer
            lcd_printf("Distance: %lf \n",sum);
            lcd_init();
            sensor_data -> distance = 0;
        }

        oi_setWheels(0,0);
        return sum;
}

void bump_left(oi_t *sensor_data){
    move_backward(sensor_data, 50);
    turn_right(sensor_data, 90);
    move_forward_no_bump(sensor_data, 180);
    turn_left(sensor_data, 90);
}

void bump_right(oi_t *sensor_data){
    move_backward(sensor_data, 50);
    turn_left(sensor_data, 90);
    move_forward_no_bump(sensor_data, 180);
    turn_right(sensor_data, 90);
}


double move_backward(oi_t *sensor_data, double distance_mm){

    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(-250,-250); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);
        sum -= sensor_data -> distance; // use -> notation since pointer
        lcd_init();
    }

    oi_setWheels(0,0);
    return sum;
}

double turn_right(oi_t *sensor_data, double degrees) {
   double startingAngle = sensor_data -> angle;
   double currentAngle = startingAngle;

   oi_setWheels(250,-250);

   while(currentAngle < (startingAngle + degrees)) {
       oi_update(sensor_data);
       currentAngle += sensor_data -> angle;
   }
   oi_setWheels(0,0);
    return currentAngle;
}

double turn_left(oi_t *sensor_data, double degrees) {
    double startingAngle = sensor_data -> angle;
       double currentAngle = startingAngle;

       oi_setWheels(-250,250);

       while(currentAngle > (startingAngle - degrees)) {
           oi_update(sensor_data);
           currentAngle += sensor_data -> angle;
       }
       oi_setWheels(0,0);
        return currentAngle;
}
