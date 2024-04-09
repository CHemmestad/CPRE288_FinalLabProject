/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"

volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile unsigned long RESULTS = 0;
volatile int OVERFLOW = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= 0x02;
    SYSCTL_RCGCTIMER_R |= 0x08;
    while ((SYSCTL_PRGPIO_R & 0x02) == 0) {};
    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R &= 0xFFFF0FFF;
    GPIO_PORTB_PCTL_R |= 0x00007000;

    TIMER3_CTL_R &= ~0x0100;
    TIMER3_CFG_R = 0x04;
    TIMER3_TBMR_R &= 0xFFFFFFEF;
    TIMER3_TBMR_R |= 0x07;
    TIMER3_CTL_R |= 0x0C00;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_IMR_R |= 0x0400;

    NVIC_PRI9_R = (NVIC_PRI9_R & 0xFFFFFF0F) | 0x00000020;
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R |= 0x0100;
}

void ping_trigger (void){
    STATE = LOW;

    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x0100;
    TIMER3_IMR_R &= ~0x0400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x08;

    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_DATA_R &= ~0b1000;
    GPIO_PORTB_DATA_R |= 0b1000;
  //  timer_waitMillis(1);
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0b1000;
    GPIO_PORTB_DIR_R &= ~0x08;
    //timer_waitMillis(18);


    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x0400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x0400;
    TIMER3_CTL_R |= 0x0100;

    while(STATE != DONE){}
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.
    //if(TIMER3_RIS_R & 0x0400) {

        TIMER3_ICR_R |= 0x0400;
        switch(STATE) {
        case LOW:
            STATE = HIGH;
            START_TIME = TIMER3_TBR_R;
            break;
        case HIGH:
            STATE = DONE;
            END_TIME = TIMER3_TBR_R;
            ping_getDistance();
            break;
        }
    //}
}

void ping_getDistance (void){

    // YOUR CODE HERE

    RESULTS = START_TIME - END_TIME;
    if(END_TIME > START_TIME) {
        OVERFLOW = 1;
        RESULTS += 16777216;
    }

}
