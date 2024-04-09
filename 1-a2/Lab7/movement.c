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


float values[VALUES_SIZE];
Object objects[VALUES_SIZE];
double pointDist = DBL_MAX;
int pointAngInd;
int driveInd;
int hasBumped = 1;
//0 means left, 1 means right
int objectDirection = 3;
float distanceValues[90];
int smallestObjectAngle;
float smallestObjectDistance;
int timesBumped = 0;

int main(void) {

    //Initializing code
    timer_init();
    lcd_init();
    uart_init();
    cyBOT_init_Scan(0b0111);
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    //cyBOT_SERVO_cal();
    /*
     * 280000 and 1256500 for cybot 5 in tla, need to recalibrate for upstairs lab
     */


    right_calibration_value = 274750;
    left_calibration_value = 1293250;

    char input;

    while(true){
        input = uart_receive_nonblocking();

        if(input == 'w'){
            getMessage(input);
            move_forward(sensor_data, 250);
        }
        else if(input == 'q'){
            getMessage(input);
                findObjects();
                timer_waitMillis(500);
                driveToSmallest(sensor_data);
                break;
        }
        else if(input == 'a'){
            getMessage(input);
            turn_left(sensor_data, 90);
        }
        else if(input == 'x'){
            getMessage(input);
            move_backward(sensor_data, 250);
        }
        else if(input == 'd'){
            getMessage(input);
            turn_right(sensor_data, 90);
        }
        else if(input == 'm'){
            getMessage(input);
            findObjects();
        }
        else if(input == 'e'){
            getMessage(input);
            oi_free(sensor_data);
            break;
        }
    }
    return 0;
 }

void findObjects() {
    //scan surroundings
    scanStuff();


    int objectCount = 0;
    int i;
    int angle;
    int distance;
    int previousDistance;
    int start_angle;
    int end_angle;
    int maximumObjectDistance = 850;
    float widthInRadians;
    int degreeWidth;

    for(i = 0; i < 90; i++){
        angle = i;
        distance = values[i];

        if(i == 0){
            previousDistance = 0;
        } else {
            previousDistance = values[i-1];
        }


        if((distance > maximumObjectDistance) && (previousDistance < maximumObjectDistance)){

                objects[objectCount].start_angle = angle * 2;
                start_angle = angle * 2;

        } else if((previousDistance > maximumObjectDistance) && (distance < maximumObjectDistance)){
            objects[objectCount].ending_angle = (angle * 2) - 2;
            end_angle = (angle * 2)  - 2;
            objects[objectCount].radial_width = end_angle - start_angle;
            objectCount++;
        }


    }

    cyBOT_Scan_t scan;
    int midpointAngle;
    for(i = 0; i < objectCount; i++){
        midpointAngle = (objects[i].ending_angle + objects[i].start_angle)/2;

        //Scan twice otherwise value is wrong
        cyBOT_Scan(midpointAngle, &scan);
        timer_waitMillis(500);
        cyBOT_Scan(midpointAngle, &scan);
        objects[i].closest_distance = scan.sound_dist;

        degreeWidth = objects[i].ending_angle - objects[i].start_angle;
        objects[i].radial_width = degreeWidth;
        widthInRadians = (float)(degreeWidth) * (M_PI/180);
        objects[i].linear_width = widthInRadians * objects[i].closest_distance;

    }


    //display statistics
    int j;
    char message[104];
    for (j =  0; j < objectCount; j++) {
        if(objects[j].radial_width != 0 && objects[j].radial_width > 3){
            sprintf(message, "Object from %d degrees to %d degrees, Distance: %.2fcm, linear width %.2fcm\r\n", objects[j].start_angle, objects[j].ending_angle, objects[i].closest_distance ,objects[j].linear_width);
            uart_sendStr(message);
        }
    }

    int smallestWidth = 100;
    for(i = 0; i < objectCount; i++){
        if((objects[i].radial_width <= smallestWidth) && (objects[i].radial_width > 3)){
            smallestWidth = objects[i].radial_width;
            smallestObjectAngle = (objects[i].ending_angle + objects[i].start_angle)/2;
            smallestObjectDistance = objects[i].closest_distance - 10;
        }
    }
        cyBOT_Scan(smallestObjectAngle, &scan);



//    int start_angle = -1;
//    double closest_distance = DBL_MAX;
//    int closest_angle = -1;
//    double previous_distance = DBL_MAX;
//    const int scanDistance = 900;
//    int i;
//
//    //search for objects
//    for(i = 0; i < VALUES_SIZE; i++){
//        int angle = i * 2;
//        double distance = values[i];
//
//        //detect whether object is within distance and values are within 5cm of each other
//        if(distance > scanDistance && fabs(distance - previous_distance) <= 200){
//            if(start_angle == -1){
//                start_angle = angle;
//            }
//            if(distance < closest_distance){
//                closest_distance = distance;
//                closest_angle = angle;
//            }
//        }
//        else{
//            //set values
//            if(start_angle != -1){
//                objects[objectCount].start_angle = start_angle;
//                objects[objectCount].closest_angle = closest_angle;
//                objects[objectCount].closest_distance = closest_distance;
//                objects[objectCount].ending_angle = angle;
//                objects[objectCount].radial_width = objects[objectCount].ending_angle - objects[objectCount].start_angle;
//                objectCount++;
//
//                start_angle = -1;
//                closest_distance = DBL_MAX;
//                closest_angle = -1;
//            }
//        }
//        previous_distance = distance;
//    }
//
//    //set values
//    if(start_angle != -1){
//        if(start_angle != -1 && 178 - start_angle > 6){
//            objects[objectCount].start_angle = start_angle;
//            objects[objectCount].closest_angle = closest_angle;
//            objects[objectCount].closest_distance = closest_distance;
//            objects[objectCount].ending_angle = 178 - start_angle;
//            objects[objectCount].radial_width = objects[objectCount].ending_angle - objects[objectCount].start_angle;
//            objectCount++;
//        }
//    }
//    //find linear width
//    int o;
//    for(o = 0; o < objectCount; o++){
//        objects[objectCount].radial_width = objects[objectCount].ending_angle - objects[objectCount].start_angle;
//        scanMedianDist(objects[o].start_angle, objects[o].ending_angle, o);
//
//        objects[o].linear_width = 2 * (M_PI) * objects[o].closest_distance * (objects[o].radial_width / 360.0);
//    }
//
//
//    //display statistics
//    int j;
//    char message[104];
//    for (j =  0; j < objectCount; j++) {
//        if(objects[j].radial_width != 0 && objects[j].radial_width > 3){
//            sprintf(message, "Object from %d degrees to %d degrees, closest distance %.2fcm at %d degrees, linear width %.2fcm\r\n", objects[j].start_angle, objects[j].ending_angle, objects[j].closest_distance, objects[j].closest_angle, objects[j].linear_width);
//            uart_sendStr(message);
//        }
//    }
//
//
//    //point scanner at closest object
//    int k;
//    for(k = 0; k < objectCount; k++){
//        if(objects[k].linear_width < pointDist && objects[k].radial_width > 3){
//            pointDist = objects[k].linear_width;
//            pointAngInd = objects[k].closest_angle;
//            driveInd = k;
//        }
//    }
    /*
    char thingy[50];
    sprintf(thingy, "%d", pointAngInd);
    printToPutty(thingy);
    cyBOT_Scan(pointAngInd, &scan);*/
}

void scanStuff(){

    int i;
    int x1;
    int x2;
    char message[] = "\rDegrees      IR (ADC) \n\r";
    char angle[100];
    char distance[100];
    uart_sendStr(message);


    cyBOT_Scan_t scan;
    cyBOT_Scan(0, &scan);
    timer_waitMillis(25);
    for(i = 0; i <= 180; i+=2){
        cyBOT_Scan(i, &scan);
        x1 = scan.IR_raw_val;

        cyBOT_Scan(i, &scan);
        x2 = scan.IR_raw_val;

       values[i / 2] = ((x1 + x2) / 2);


        sprintf(angle, "%d", i);
        uart_sendStr(strcat(angle, "     "));
        uart_sendStr("\t");
        sprintf(distance, "%g", values[i / 2]);
        uart_sendStr(strcat(distance, "     "));
        uart_sendStr("\r\n");
    }
}

void scanMedianDist(int startAngle, int endingAngle, int objNum){
    cyBOT_Scan_t scan;
    int medianAngle = (startAngle + endingAngle) / 2;
    float d1;
    float d2;
    float d3;

    cyBOT_Scan(medianAngle, &scan);
    timer_waitMillis(100);
    cyBOT_Scan(medianAngle, &scan);
    d1 = scan.sound_dist;
    cyBOT_Scan(medianAngle, &scan);
    d2 = scan.sound_dist;
    cyBOT_Scan(medianAngle, &scan);
    d3 = scan.sound_dist;
    objects[objNum].closest_distance = (d1 + d2 + d3) / 3;
    objects[objNum].closest_angle = medianAngle;

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
    hasBumped = 1;
}

void bump_right(oi_t *sensor_data){
    move_backward(sensor_data, 50);
    turn_left(sensor_data, 90);
    move_forward_no_bump(sensor_data, 180);
    turn_right(sensor_data, 90);
    hasBumped = 1;
}

double move_forward(oi_t *sensor_data, double distance_mm){

    double sum = 0; // distance member in oi_t struct is type double
    oi_setWheels(250,250); //move forward at full speed

    while (sum < distance_mm) {
        oi_update(sensor_data);
        hasBumped = 0;
        if(sensor_data -> bumpLeft || sensor_data -> bumpRight){
            if(objectDirection == 1){
                bump_right(sensor_data);
                move_forward_no_bump(sensor_data, (distance_mm - sum) * 2);
            } else if(objectDirection == 0){
                bump_left(sensor_data);
                move_forward_no_bump(sensor_data, (distance_mm - sum) * 2);
            }
            hasBumped = 1;
            timesBumped++;
            return 0;
        }
        sum += sensor_data -> distance; // use -> notation since pointer
        lcd_printf("Distance: %lf \n",sum);
        lcd_init();
        sensor_data -> distance = 0;
    }

    oi_setWheels(0,0);
    return sum;
}

void driveToSmallest(oi_t *sensor_data){
    int angleTurnAmount;

   while(hasBumped == 1){

       if(timesBumped > 0){
           findObjects();
       }

    if(smallestObjectAngle < 90) {
        angleTurnAmount = (90 - smallestObjectAngle) / 2;
        objectDirection = 1;
        turn_left(sensor_data, angleTurnAmount);
    } else if(smallestObjectAngle > 90) {
        angleTurnAmount = (smallestObjectAngle - 90) / 2;
        objectDirection = 0;
        turn_right(sensor_data, angleTurnAmount);
    } else {
        angleTurnAmount = 0;
    }

    int distanceToDrive = (smallestObjectDistance * 10) - 100;

    if(distanceToDrive > 100){
    move_forward(sensor_data, distanceToDrive);
    } else {
        hasBumped = 0;
    }

   }


//    //cyBOT_Scan_t scan;
//    int distanceAngle;
//    char angle[50];
//    //sprintf(angle, "%f\r\n", angle);
//    uart_sendStr(angle);
//    //check to see if robot is facing closest object
//   distanceAngle = 90 - pointAngInd;
//   if(distanceAngle > 0){
//       turn_left(sensor_data, distanceAngle - (distanceAngle / 12));
//       move_forward(sensor_data, (objects[driveInd].closest_distance * 10) - 100);
//       oi_free(sensor_data);
//   }
//   else if(distanceAngle < 0){
//       turn_right(sensor_data, fabs(distanceAngle) / 2);
//   }
//    //move forward to closest object(times distance by 10 to covert to mm from cm)
//    move_forward(sensor_data, (objects[driveInd].closest_distance - 20) * 10);
//    //cyBOT_Scan(180, &scan);
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
