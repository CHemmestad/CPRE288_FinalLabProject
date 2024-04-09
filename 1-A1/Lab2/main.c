

/**
 * main.c
 */
#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "lcd.h"

int main(void) {
    oi_t *sensor_data = oi_alloc();
    //oi_t *sensor_data = oi_alloc(); // do this only once at start of main()
    oi_init(sensor_data);
    //oi_init(sensor_data); // do this only once at start of main()
    // the following code could be put in function move_forward()
    lcd_init();
    timer_init();

    timer_waitMillis(5000);

    //oi_setWheels(0,0); //stop

    int x = 0;
    for(; x < 4; x++) {
        moveForward(sensor_data, 500);
        turnRight(sensor_data, 90);
    }

//    double sum = 0; // distance member in oi_t struct is type double
//    lcd_printf("%.2lf", sum);
//
//    oi_setWheels(100,100); //move forward at full speed
//
//    while (sum < 1000) {
//        oi_update(sensor_data);
//        sum += sensor_data -> distance; // use -> notation since pointer
//        lcd_clear();
//        lcd_printf("%.2lf", sum);
//    }

    oi_free(sensor_data); // do this once at end of main()
    return 0;
}


