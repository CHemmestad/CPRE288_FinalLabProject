/*
 * movement.c
 *
 *  Created on: Jan 30, 2024
 *      Author: kadenb
 */
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"

double move_forward(oi_t *sensor_data, double distance_mm);
double turn_right(oi_t *sensor_data, double degrees);
double turn_left(oi_t *sensor_data, double degrees);
double move_backward(oi_t *sensor_data, double distance_mm);

void main() {
    timer_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data); // do this only once at start of main()
// the following code could be put in function move_forward()
    move_forward(sensor_data, 2000);
    oi_free(sensor_data); // do this once at end of main()

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
