

/**
 * main.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "driverlib/interrupt.h"
#include "uart-interrupt.h"

typedef struct Object{
    int number;
    int startAngle;
    int endAngle;
    int centerAngle;
    int angularWidth;
    int linearWidth;
    float closestDistance;
} Object;

int findSmallest(Object object[], int objects) {
    int smallest;
    int smallestObject;
    int i = 0;
    smallest = object[i].linearWidth;
    smallestObject = i;
    for(; i < objects; i++) {
        if(object[i].linearWidth < smallest) {
            smallest = object[i].linearWidth;
            smallestObject = i;
        }
    }
    return smallestObject;
}

void printString(char string[]) {
    int j;
    for(j = 0; j < strlen(string); j++) {
        uart_sendChar(string[j]);
    }
}

int findRadialWidth(int start, int end) {
    return end - start;
}

int findCenterAngle(int start, int end) {
    return (start + end)/2;
}

void printObjectData(Object object[], int i) {
    char string[100];
    sprintf(string, "\r\nObject number is: %d\n", object[i].number);
    printString(string);
    sprintf(string, "\rStart angle is: %d\n", object[i].startAngle);
    printString(string);
    sprintf(string, "\rEnd angle is: %d\n", object[i].endAngle);
    printString(string);
    sprintf(string, "\rShortest distance: %.2f\n", object[i].closestDistance);
    printString(string);
    sprintf(string, "\rCenter Angle is: %d\n", object[i].centerAngle);
    printString(string);
    sprintf(string, "\rAngular width is: %d\n", object[i].angularWidth);
    printString(string);
    sprintf(string, "\rLinear width is: %d\n", object[i].linearWidth);
    printString(string);
}

void findClosest(float prev, float current, Object object[], int z) {
    if(current < prev) {
        object[z].closestDistance = current;
    }
}

int objectDetected(int prev, int current) {
    if(prev - current < -125) {
        return 1;
    } else {
        return 0;
    }
}

int objectLeft(int prev, int current) {
    if(prev - current > 100) {
        return 1;
    } else {
        return 0;
    }
}

float getAverage(cyBOT_Scan_t* data, int angle) {
    float average = 0;
    int i = 0;
    for(; i < 2; i++) {
        cyBOT_Scan(angle, data);
        average += data->IR_raw_val;
    }
    average = average/2;
    return average;
}

void calculateLinearWidth(Object object[], int i) {
    object[i].linearWidth = 2 * 3.1415 * object[i].closestDistance * (object[i].angularWidth / 360.00);
}

void initialize(Object object[], int i) {
    int j = 0;
    for(; j < i; j++) {
        object[j].angularWidth = 0;
        object[j].centerAngle = 0;
        object[j].closestDistance = 0;
        object[j].endAngle = 0;
        object[j].startAngle = 0;
        object[j].linearWidth = 0;
        object[j].number = 0;
    }
}

int main(void) {


    cyBot_uart_init_clean();
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    timer_init();
    lcd_init();

    //cyBOT_SERVO_cal();
    right_calibration_value = 238000;
    left_calibration_value = 1235500;

    oi_setWheels(0,0); //stop

    cyBOT_Scan_t* data = calloc(1, sizeof(cyBOT_Scan_t));
    Object object[10];
    initialize(object, 10);
    float prev = 0;
    float current = 0;
    int seen = 0;
    int z = 0;

    while(1) {
        char character = (char) uart_receive();
        lcd_putc(character);

        cyBOT_Scan(90, data);
        if(character == 'm') {
            printString("\rDegrees\tPING Distance (cm)\n");
            int i;
            char buffer[10];
            for(i = 0; i <= 180; i += 2) {
                cyBOT_Scan(i, data);
                prev = current;
                current = getAverage(data, i);
                if(objectDetected(prev, current)) {
                    printString("\rObject detected\n");
                    object[z].startAngle = i;
                    object[z].number = z + 1;
                    seen = 1;
                }
                if(seen) {
                    findClosest(prev, current, object, z);
                }
                if((objectLeft(prev, current) && seen) || (seen && i == 180)) {
                    printString("\rObject left\n");
                    object[z].endAngle = i;
                    object[z].angularWidth = findRadialWidth(object[z].startAngle, object[z].endAngle);
                    object[z].centerAngle = findCenterAngle(object[z].startAngle, object[z].endAngle);
                    printObjectData(object, z);
                    z = z + 1;
                    seen = 0;
                }
                sprintf(buffer, "\r%4d\t%10.2f\n", i, current);
                printString(buffer);
            }

            int a = 0;
            for(; a < z; a++) {
                cyBOT_Scan(object[a].centerAngle, data);
                timer_waitMillis(2000);
                cyBOT_Scan(object[a].centerAngle, data);
                object[a].closestDistance = data->sound_dist;
                calculateLinearWidth(object, a);
                printObjectData(object, a);
                timer_waitMillis(2000);
            }
            int objectIndex = findSmallest(object, z);

            moveForward(sensor_data, 110);

            if(object[objectIndex].centerAngle < 90) {
                turnRight(sensor_data, object[objectIndex].centerAngle);
            } else if(object[objectIndex].centerAngle > 90) {
                turnLeft(sensor_data, object[objectIndex].centerAngle - 90);
            }

            moveForward(sensor_data, object[objectIndex].closestDistance*10 - 350);

        } else if(character == 'z') {
            break;
        } else if(character == 'a') {
            turnLeft(sensor_data, 5);
        } else if(character == 'd') {
            turnRight(sensor_data, 5);
        } else if(character == 'w') {
            moveForward(sensor_data, 250);
        } else if(character == 's') {
            moveBack(sensor_data, 250);
        }
        cyBOT_Scan(90, data);
    }

    free(sensor_data);
    free(data); // do this once at end of main()
    return 0;
}
