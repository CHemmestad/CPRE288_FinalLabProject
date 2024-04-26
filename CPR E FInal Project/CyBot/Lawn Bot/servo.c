#include "servo.h"
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
    float val_0 = 19.93;          
    float val_90 = 18.5;      
    float val_180 = 18.18;
    double pulseWidth;
    int matching;
    double test;


    float intercept = 0 ;           
    float slope = ( (val_0 - val_180) / (180));              
    intercept = val_0;                      
    float degree1 = degrees;

    pulseWidth = (intercept - slope * degree1) ;    


    matching = pulseWidth * 16000;

    TIMER1_TBMATCHR_R = (matching & 0xFFFF);                          
    TIMER1_TBPMR_R = ((matching >> 16) & 0xFF);  

    timer_waitMillis(20);
}

void servo_calibrate(){
    button_init();
    double pulseWidth = 19;
    uint8_t buttonVal;
    int matching;
    double val_180 = 0;
    double val_0 = 0;

    while(1) {
        lcd_printf("1:L , 2:R, 3/4:val\nPulse Width: %.2f\n0 value:%.2f\n180 value:%.2f", pulseWidth, val_0, val_180);
        buttonVal = button_getButton();

        switch(buttonVal){
            case 1:
            pulseWidth -= 0.01;
            break;
            case 2:
            pulseWidth += 0.01;
            break;
            case 3:
            val_180 = pulseWidth;
            break;
            case 4:
            val_0 = pulseWidth;
            break;
        }
        

        matching = pulsewidth * 16000;
        TIMER1_TBMATCHR_R = (matching & 0xFFFF);                           //Lower 16 Match register - set to low 16 bits of desired val
        TIMER1_TBPMR_R = ((matching >> 16) & 0xFF);                        //Prescale match register - set to high 8 of desired val
        timer_waitMillis(20);


    }
}


