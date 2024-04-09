/*
 * movement.c
 *
 *  Created on: Jan 30, 2024
 *      Author: kadenb
 */
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"

struct objectsFound {
    int number;
    int radius;
    int angle;
    float distance;
};

double move_forward(oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor_data, double degrees);
double turn_left(oi_t *sensor_data, double degrees);
double move_backward(oi_t *sensor_data, double distance_mm);
void getMessage(char message);
void getPingData(char message, char data[]);
void printObjectsData(int objectNumber, int objectAngle, float objectDistance, int objectRadius);



struct object{
    int number;
    int angle;
    float distance;
    int radius;
};


struct object objects[10];

int main() {

    //Initializing code
    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0011);
    //oi_t *sensor_data = oi_alloc();
    //oi_init(sensor_data);
    //cyBOT_SERVO_cal();
    /*
     * 280000 and 1256500 for cybot 5 in tla, need to recalibrate for upstairs lab
     */


    right_calibration_value = 285250;
    left_calibration_value = 1251250;

    int i;
    cyBOT_Scan_t scan;


    int input;
    char totalData[2000];
    float distanceValue;
    char currentData[200];
    float lowestDistanceValue;
    int lowestDistanceAngle;
    int objectDetected = 0;
    int objectsDetected = 0;
    int objectDistance;
    float tempDist;
    int startingAngle;
    int endingAngle;
    int radius;
//    struct objectsFound objectOne;
//    struct objectsFound objectTwo;
    char objectsMessage[200];




    while(1){
        input = cyBot_getByte();

        if(input == 126) {
            break;
        }

        lcd_printf("%c", input);

        if(input > 32 && input < 127) {

            lcd_printf("%c", input);
            getMessage(input);

            if(input == 109){
                for(i = 0; i <=180; i+=2) {

                    //Start scan and write data values to a string
                        cyBOT_Scan(i, &scan);
                        distanceValue = scan.sound_dist;

                        if(i == 0) {
                            lowestDistanceValue = 500;
                         } else if(distanceValue < lowestDistanceValue){
                             lowestDistanceValue = distanceValue;
                             lowestDistanceAngle = i;
                         }


                        if(distanceValue < 75 && objectDetected == 0) {
                            objectDetected = 1;
                            objectDistance = distanceValue;
                            startingAngle = i;

                        } else if(distanceValue > 75 && objectDetected == 1) {
                            objectDetected = 0;
                            objectsDetected++;
                            endingAngle = i;
                            radius = endingAngle - startingAngle;

                            objects[objectsDetected].number = objectsDetected;
                            objects[objectsDetected].angle = lowestDistanceAngle;
                            objects[objectsDetected].distance = lowestDistanceValue;
                            objects[objectsDetected].radius = radius;


//                            if(objectsDetected == 1){
//                                objectOne.angle = radius / 2;
//                                objectOne.number = 1;
//                                objectOne.radius = radius;
//                                objectOne.distance = lowestDistanceValue;
//                            }
//                            else if(objectsDetected == 2){
//                                objectTwo.angle = radius / 2;
//                                objectTwo.number = 2;
//                                objectTwo.radius = radius;
//                                objectTwo.distance = lowestDistanceValue;
//                            }
                        }


                        lcd_printf("%.4f", distanceValue);

                        //fixes weird issue with starting i printing out weird characters
                        if(i == 0) {
                            sprintf(currentData, "0\t\t%.4f \n\r", distanceValue);
                        }
                        sprintf(currentData, "%d\t\t%.4f \n\r", i, distanceValue);
                        strncat(totalData, currentData, 100);
                    }

                sprintf(objectsMessage, "Amount of objects detected: %d \n\r", objectsDetected);
                getPingData(input, totalData);
                cyBOT_Scan(lowestDistanceAngle, &scan);

                for(i = 0; i < strlen(objectsMessage); i++){
                    cyBot_sendByte(objectsMessage[i]);
                }


//                printObjectsData(objectOne.number, objectOne.angle, objectOne.distance, objectOne.radius);
//                printObjectsData(objectTwo.number, objectTwo.angle, objectTwo.distance, objectTwo.radius);
//                cyBot_sendByte(objectOne.number);
//                cyBot_sendByte(objectTwo.number);

                for(i = 1; i <= objectsDetected; i++){
                    printObjectsData(objects[i].number, objects[i].angle, objects[i].distance, objects[i].radius);
                }


            }

        }

    }






    //oi_free(sensor_data); // do this once at end of main()
    return 0;
 }

void printObjectsData(int objectNumber, int objectAngle, float objectDistance, int objectRadius){
    char objectInfo[150];

    sprintf(objectInfo, "Object %d: Angle:%d Distance:%.4f Radius:%d \n\r", objectNumber, objectAngle, objectDistance, objectRadius);
    int i;
    for(i = 0; i < strlen(objectInfo); i++){
        cyBot_sendByte(objectInfo[i]);
    }
}

void getPingData(char message, char data[]){

    char dataInfo[] = "\rDegrees      PING Distance (cm) \n\r";
    int i;

    for(i = 0; i < strlen(dataInfo); i++){
           cyBot_sendByte(dataInfo[i]);
        }

    for(i = 0; i < strlen(data); i++){
        cyBot_sendByte(data[i]);
    }


    return;
}

void getMessage(char message){
    char tempMsg[100];

    sprintf(tempMsg, "Got a message %c \n\r", message);
    //printf("Got a %s", tempMsg);

    int i;
    for(i = 0; i < strlen(tempMsg); i++){
       cyBot_sendByte(tempMsg[i]);
    }

}

double move_forward(oi_t *sensor_data, double distance_mm){

    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(250,250); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);
        if(sensor_data -> bumpLeft) {
            move_backward(sensor_data, 150);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, (distance_mm - sum));
        } else if(sensor_data -> bumpRight) {
            move_backward(sensor_data, 150);
            turn_right(sensor_data, 90);
            move_forward(sensor_data, 250);
            turn_left(sensor_data, 90);
            move_forward(sensor_data, (distance_mm - sum));
        }
        sum += sensor_data -> distance; // use -> notation since pointer
        lcd_printf("Distance: %lf \n",sum);
        lcd_init();
        sensor_data -> distance = 0;
    }

    oi_setWheels(0,0);
    return sum;
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
