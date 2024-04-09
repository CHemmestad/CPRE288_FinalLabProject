#include <servo.h>
#include <lcd.h>
#include <Timer.h>

#define INT_UART1 22

int main(void){
    timer_init();
    lcd_init();
    servo_init();

    servo_move(0);
    timer_waitMillis(1000);
    servo_move(90);
    timer_waitMillis(500);
    servo_move(180);
    timer_waitMillis(1000);
    servo_move(45);

    return 0;
}
