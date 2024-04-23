#include  "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "button.h"
#include "stdio.h"

unsigned long pwm_period = 0x4E200;



void servo_init(){
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R |= 0x02;
    while ((SYSCTL_PRGPIO_R & 0x02) != 0x02) {};
    GPIO_PORTB_DIR_R &= ~0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;
    GPIO_PORTB_PCTL_R |= 0x00700000;
    GPIO_PORTB_DEN_R |= 0x20;

    //enable pwm mode
    TIMER1_CTL_R &= ~0x100; //disable timer1b
    TIMER1_CFG_R = 0x4; //configure to 16-bit mode

    TIMER1_TBPR_R = pwm_period >> 16; //extend timer range to 16bit
    TIMER1_TBILR_R = pwm_period & 0xFFFF; //determine period of pwm

    TIMER1_TBMR_R = 0x0A;//enable pwm / configure for periodic mode.
}

uint16_t servo_move(uint16_t degrees){
    float pulseCalculation = (degrees/180.0) + 1.0;
    //long matchValue = (320000 - (pulseCalculation * 16000));
    long matchValue = (320000 - (long)(pulseCalculation * 16000));

   // long pulseCalculation = ((degrees/180.0) + 1.0)/;


    TIMER1_TBMATCHR_R = matchValue & 0x00FFFF;
    TIMER1_TBPMR_R = matchValue >> 16;

    TIMER1_CTL_R |= 0x100; //enable timer1b
    timer_waitMillis(500);
    TIMER1_CTL_R &= ~0x100;

    return degrees;
}

void servo_calibrate(){
    servo_move(90);

    int dir = 1;
    int cnt = 0;
    char msg[90];

    while(1){
        int button = button_getButton();
        switch(button){
        case 1:
        TIMER1_CTL_R |= 0x100;
        TIMER1_TBMATCHR_R += 10 * dir;
        break;

        case 2:
        TIMER1_CTL_R |= 0x100;
        TIMER1_TBMATCHR_R += 100 * dir;
        break;

        case 3:
        TIMER1_CTL_R |= 0x100;
        if(cnt == 0){
            dir = -1;
            cnt = 1;
        }
        else if(cnt == 1){
            dir = 1;
            cnt = 0;
        }

        break;

        case 4:
        TIMER1_CTL_R |= 0x100;
        TIMER1_TBMATCHR_R -= 100;
        break;

        default:
        timer_waitMillis(100);
        TIMER1_CTL_R &= ~0x100;
        break;
        }

        if(cnt == 0){
            sprintf(msg, "Match Value:\n%d \nDirection:\nclockwise", TIMER1_TBMATCHR_R);
        }
        else if(cnt == 1){
            sprintf(msg, "Match Value:\n%ud \nDirection:\nc-clockwise", TIMER1_TBMATCHR_R);
        }
        lcd_printf("%s", msg);
    }
}


