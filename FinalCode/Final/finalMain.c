

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
#include "sound.h"
#include "imu.h"
#include "i2c.h"
#include "bno055.h"
#include "servo.h"
#include "uart.h"
#include "adc.h"
#include "button.h"

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
    smallest = object[i].angularWidth;
    smallestObject = i;
    for(; i < objects; i++) {
        if(object[i].angularWidth < smallest) {
            smallest = object[i].angularWidth;
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

//float getAverage(int angle) {
//    float average = 0;
//    int i = 0;
//    for(; i < 4; i++) {
//        average += adc_read();
//    }
//    average = average/4;
//    return average;
//}

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

void makeString(char* string, int number) {
    *(string + 3) = '\0';
    *(string + 2) = (char)((number % 10) + 48);
    *(string + 1) = (char)(((number / 10) % 10) + 48);
    *(string + 0) = (char)(((number / 100) % 10) + 48);
}

//void playZelda() {
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);
//    int index = 0;
//    int numNotes = 63;
//    unsigned char notes[63] = {69, 69, 69, 69, 69, 67, 69, 0, 69, 69, 69, 69, 69, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 69, 64, 69, 69, 71, 73, 74,
//    76, 0, 76, 76, 77, 79, 81, 0, 81, 81, 79, 77, 79, 77, 76, 76, 74, 74, 76, 77, 76, 74, 72, 72, 74, 76, 74, 72, 71, 71, 73, 75, 78};
//    unsigned char duration[63] = {16, 16, 16, 16, 32, 16, 32, 32, 16, 16, 16, 16, 16, 8, 8, 16, 8, 8, 16, 8, 8, 16, 16, 32, 50, 16, 8, 8, 8, 8,
//    64, 16, 16, 16, 16, 16, 64, 16, 16, 16, 16, 16, 24, 8, 64, 32, 16, 8, 8, 64, 16, 16, 16, 8, 8, 64, 16, 16, 16, 8, 8, 64, 32};
//    oi_loadSong(index, numNotes, notes, duration);
//    oi_play_song(index);
//    free(sensor_data);
//}
//
//void playR2D2(int note) {
//    oi_t *sensor_data = oi_alloc();
//    oi_init(sensor_data);
//    int index = 0;
//    int numNotes = 4;
//    unsigned char notes[4] = {note, note, note, note};
//    unsigned char duration[4] = {32, 32, 32, 32};
//    oi_loadSong(index, numNotes, notes, duration);
//    oi_play_song(index);
//    free(sensor_data);
//}
int calculateDistance(int value) {
    int results = round(1.0 / ((value / 4095.0) * .13));
    if(results > 80) {
        return 80;
    } else {
        return results;
    }
}

int main(void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    timer_init();
    lcd_init();
    bno_init();
    adc_init();
    uart_init();
    servo_init();
    imu_init();
    imu_writeReg(IMU_OPR_MODE, NDOF);
//    imu_setMode(MAGONLY);
    imu_setDefaultUnits();

    bno_t* bno = bno_alloc();

//    cyBOT_SERVO_cal();
//    servo_calibrate();
//    right_calibration_value = 248500;
//    left_calibration_value = 1167250;

    oi_setWheels(0,0); //stop

//    cyBOT_Scan_t* data = calloc(1, sizeof(cyBOT_Scan_t));
    float heading;
    float adj = 0;
//    mag_t* mag;
//    mag = imu_getMag();

    while(1) {
        char character = (char)uart_receive();
        if(character == 'm') {
            int i = 0;
            servo_move2(i);
            timer_waitMillis(1000);
            for(; i <= 180; i += 2) {
                servo_move2(i);
        //        cyBot_sendByte(i);
        //        cyBot_sendByte(calculateDistance(data->IR_raw_val));
                char angle[3];
        //        makeString(angle, i);
                sprintf(angle, "%3d", i);
                printString(angle);
                char distance[2];
                sprintf(distance, "%2d", (int)calculateDistance(adc_read()));
                printString(distance);
            }
        }
        if(character == 'p') {
            playZelda();
//            playRickRoll();
        }
        if(character == 'w') {
            int distance = (int)moveForwardWithDetect(sensor_data, 30);
            bno_update(bno);
            heading = (bno->euler.heading / 16.0);
            heading -= adj;
            lcd_printf("%f", heading);
            if(heading < 0) {
                heading = (int)(heading + 360) % 360;
            }
            char string[3];
            sprintf(string, "%3d", (int)heading);
            printString(string);
            sprintf(string, "%3d", distance);
            printString(string);
        }
        if(character == 's') {
            int distance = (int)moveBack(sensor_data, 30);
            bno_update(bno);
            heading = (bno->euler.heading / 16.0);
            heading -= adj;
            if(heading < 0) {
                heading = (int)(heading + 360) % 360;
            }
            char string[3];
            sprintf(string, "%3d", (int)heading);
            printString(string);
            sprintf(string, "%3d", distance);
            printString(string);
        }
        if(character == 'a') {
            int angle = (int)turnLeft(sensor_data, 5);
            bno_update(bno);
            heading = (bno->euler.heading / 16.0);
            heading -= adj;
            if(heading < 0) {
                heading = (int)(heading + 360) % 360;
            }
            char string[3];
            sprintf(string, "%3d", (int)heading);
            printString(string);
        }
        if(character == 'd') {
            int angle = (int)turnRight(sensor_data, 5);
            bno_update(bno);
            heading = (bno->euler.heading / 16.0);
            heading -= adj;
            if(heading < 0) {
                heading = (int)(heading + 360) % 360;
            }
            char string[3];
            sprintf(string, "%3d", (int)heading);
            printString(string);
        }
        if(character == 'c') {
            bno_update(bno);
            adj = (int)round(bno->euler.heading / 16.0);
        }
    }


//    //Zelda music for beginning
//    int index;
//    int numNotes;
//    index = 0;
//    numNotes = 63;
//    unsigned char notes1[63] = {69, 69, 69, 69, 69, 67, 69, 0, 69, 69, 69, 69, 69, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 69, 64, 69, 69, 71, 73, 74,
//    76, 0, 76, 76, 77, 79, 81, 0, 81, 81, 79, 77, 79, 77, 76, 76, 74, 74, 76, 77, 76, 74, 72, 72, 74, 76, 74, 72, 71, 71, 73, 75, 78};
//    unsigned char duration1[63] = {16, 16, 16, 16, 32, 16, 32, 32, 16, 16, 16, 16, 16, 8, 8, 16, 8, 8, 16, 8, 8, 16, 16, 32, 50, 16, 8, 8, 8, 8,
//    64, 16, 16, 16, 16, 16, 64, 16, 16, 16, 16, 16, 24, 8, 64, 32, 16, 8, 8, 64, 16, 16, 16, 8, 8, 64, 16, 16, 16, 8, 8, 64, 32};
//    oi_loadSong(index, numNotes, notes1, duration1);
//    oi_play_song(index);

//    playZelda();
//    finishSound();
//    template();
//    moveForward(sensor_data, 100);
//    timer_waitMillis(5000);
//    template();
//    timer_waitMillis(5000);
//    template();
//    timer_waitMillis(5000);
//    template();
//    timer_waitMillis(5000);



//    for(i = 0; i < sum; i++) {
////    while(sensor_data->songPlaying) {
//        oi_play_song(index);
//    }

//    oi_loadSong(index, numNotes, notes1, duration1);
//
//    oi_play_song(index);
//    while(sensor_data->songPlaying) {
//        oi_play_song(index);
//    }

//    index = 1;
//    unsigned char notes2[16] = {0, 57, 59, 61, 62, 64, 64, 65, 67, 69, 69, 67, 65, 67, 65, 64};
//    unsigned char duration2[16] = {96, 8, 8, 8, 8, 80, 16, 16, 16, 80, 16, 16, 16, 24, 8, 80};
//    oi_loadSong(index, numNotes, notes2, duration2);
//    oi_play_song(index);
//    while(sensor_data->songPlaying) {
//        oi_play_song(index);
//    }




//    cyBOT_Scan_t* data = calloc(1, sizeof(cyBOT_Scan_t));
//    Object object[10];
//    initialize(object, 10);
//    float prev = 0;
//    float current = 0;
//    int seen = 0;
//    int z = 0;
//
//    while(1) {
//        char character = (char) cyBot_getByte();
//        lcd_putc(character);
//
//        cyBOT_Scan(90, data);
//        if(character == 'm') {
//            printString("\rDegrees\tPING Distance (cm)\n");
//            int i;
//            char buffer[10];
//            for(i = 0; i <= 180; i += 2) {
//                cyBOT_Scan(i, data);
//                prev = current;
//                current = getAverage(data, i);
//                if(objectDetected(prev, current)) {
//                    printString("\rObject detected\n");
//                    object[z].startAngle = i;
//                    object[z].number = z + 1;
//                    seen = 1;
//                }
//                if(seen) {
//                    findClosest(prev, current, object, z);
//                }
//                if((objectLeft(prev, current) && seen) || (seen && i == 180)) {
//                    printString("\rObject left\n");
//                    object[z].endAngle = i;
//                    object[z].angularWidth = findRadialWidth(object[z].startAngle, object[z].endAngle);
//                    object[z].centerAngle = findCenterAngle(object[z].startAngle, object[z].endAngle);
//                    printObjectData(object, z);
//                    z = z + 1;
//                    seen = 0;
//                }
//                sprintf(buffer, "\r%4d\t%10.2f\n", i, current);
//                printString(buffer);
//            }
//            int objectIndex = findSmallest(object, z);
//
//            int a = 0;
//            for(; a < z; a++) {
//                cyBOT_Scan(object[a].centerAngle, data);
//                timer_waitMillis(2000);
//                cyBOT_Scan(object[a].centerAngle, data);
//                object[a].closestDistance = data->sound_dist;
//                calculateLinearWidth(object, a);
//                printObjectData(object, a);
//                timer_waitMillis(2000);
//            }
//
////            moveForward(sensor_data, 110);
////
////            if(object[objectIndex].centerAngle < 90) {
////                turnRight(sensor_data, object[objectIndex].centerAngle);
////            } else if(object[objectIndex].centerAngle > 90) {
////                turnLeft(sensor_data, object[objectIndex].centerAngle - 90);
////            }
//
////            sprintf(buffer, "\r%4d\t%10.2f\n", object[objectIndex].centerAngle, data->sound_dist);
////            printString(buffer);
////            moveForward(sensor_data, data->sound_dist - 115);
//
//        } else if(character == 'z') {
//            break;
//        } else if(character == 'a') {
//            turnLeft(sensor_data, 5);
//        } else if(character == 'd') {
//            turnRight(sensor_data, 5);
//        } else if(character == 'w') {
//            moveForward(sensor_data, 250);
//        } else if(character == 's') {
//            moveBack(sensor_data, 250);
//        }
//        cyBOT_Scan(90, data);
//    }
//
//    free(sensor_data);
//    free(data); // do this once at end of main()
    return 0;
}
