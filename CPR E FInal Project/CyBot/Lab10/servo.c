/*
 * servo.c
 *
 *  Created on: Apr 2, 2024
 *      Author: cihem
 */

#include "servo.h"
#include "Timer.h"

void calcAngle(int angle);

void servo_init2(void) {
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R |= 0x02;
    while ((SYSCTL_RCGCTIMER_R & 0x02) == 0) {};

    GPIO_PORTB_DEN_R |= 0x20;
    GPIO_PORTB_DIR_R |= 0x20;
    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R &= 0xFFFF0FFF;
    GPIO_PORTB_PCTL_R |= 0x00007000;

    TIMER1_CTL_R &= ~0x0100;

    TIMER1_CFG_R = 0x04;
    //TIMER1_TBMR_R &= 0xFFFFFFEF;
    TIMER1_TBMR_R &= ~0x05;
    TIMER1_TBMR_R |= 0x08;
    TIMER1_TBMR_R |= 0x02;
    TIMER1_TBPR_R |= 0x4;
    TIMER1_TBILR_R |= 0xE200;
    TIMER1_IMR_R |= 0x0200;
    TIMER1_CTL_R |= 0x0C00;

    TIMER1_CTL_R |= 0x0100;
}

void servo_move2(uint16_t degrees) {
    TIMER1_CTL_R &= ~0x0100;
//    TIMER1_TBMATCHR_R = 0xA380;
//    TIMER1_TBPMR_R = 0x4;
//    TIMER1_TBMATCHR_R = 0xE200;
//    TIMER1_TBPMR_R = 0x4;
    calcAngle(degrees);
    TIMER1_CTL_R |= 0x0100;
    timer_waitMillis(2500);
    TIMER1_CTL_R &= ~0x0100;


//    GPIO_PORTB_DATA_R &= ~0x20;
//    GPIO_PORTB_DATA_R |= 0x20;
//    timer_waitMillis(1);
//    GPIO_PORTB_DATA_R &= ~0x20;
}

void calcAngle(int angle) {
    double ms = (angle / 180.0) + 1;
//    double clockCy = ((ms / 100) / .02) * 320000;
//    int cyclesDown = 320000 - clockCy;
    int cycLow = (20 - ms) / 6.25E-5;
    int mask = cycLow & 0xFFFF;
    TIMER1_TBMATCHR_R = mask;
    mask = cycLow >> 16;
    TIMER1_TBPMR_R = mask;
}
