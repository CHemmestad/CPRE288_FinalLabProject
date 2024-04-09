/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping_template.h"
#include "Timer.h"
#include <lcd.h>
volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse
int count = 0;

void ping_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= 0x02;

    while ((SYSCTL_RCGCGPIO_R & 0x02) == 0) {};

    //GPIO_PORTB_DIR_R &= ~0x08;
    //GPIO_PORTB_AFSEL_R |= 0b00001000;


    GPIO_PORTB_PCTL_R |= 0x00007000;
    GPIO_PORTB_PCTL_R &= 0xFFFF7FFF;
    GPIO_PORTB_DEN_R |= 0b00001000;





    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    SYSCTL_RCGCTIMER_R |= 0x8;

    while((SYSCTL_RCGCTIMER_R & 0x8) == 0){};

    TIMER3_CTL_R &= ~0x100;
    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R |= 0x07;
    TIMER3_CTL_R |= 0x0C00;

    //TIMER3_TBILR_R |= 0x8440;
    //TIMER3_TBPR_R |= 0x04;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_ICR_R |= 0x400;
    TIMER3_IMR_R |= 0x0400;
    TIMER3_CTL_R |= 0x0100;

    NVIC_EN1_R = 0x00000010;
    NVIC_PRI9_R = 0x8;



}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0b100000000;
    TIMER3_IMR_R &= ~0b100000000;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0b00001000;

    GPIO_PORTB_DIR_R |= 0b00001000;
    // YOUR CODE HERE FOR PING TRIGGER/START PULSE

    GPIO_PORTB_DATA_R &= 0x00;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R |= 0b00001000;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= 0x00;


    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0b00001000;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
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

//    char byte_received;
//        //check if handler called due to RX event
//        if (UART1_MIS_R & 0x0010)
//        {
//            //byte was received in the UART data register
//            //clear the RX trigger flag (clear by writing 1 to ICR)
//            UART1_ICR_R |= 0b00010000;
//
//            //read the byte received from UART1_DR_R and echo it back to PuTTY
//            //ignore the error bits in UART1_DR_R
//            byte_received = (char) (UART1_DR_R & 0xFF);
//            uart_sendChar(byte_received);
//
//            //if byte received is a carriage return
//            if (byte_received == '\r')
//            {
//                //send a newline character back to PuTTY
//                uart_sendChar('\n');
//            }
//            else
//            {
//                //AS NEEDED
//                //code to handle any other special characters
//                //code to update global shared variables
//                //DO NOT PUT TIME-CONSUMING CODE IN AN ISR
//
//                if (byte_received == command_byte)
//                {
//                  command_flag = 1;
//                }
//            }




  if(TIMER3_MIS_R & 0x400){
      TIMER3_ICR_R |= 0x400;
      if(STATE == LOW){
          START_TIME = TIMER3_TBR_R;
          STATE = HIGH;
      }
      else if(STATE == HIGH){
          END_TIME = TIMER3_TBR_R;
          STATE = DONE;
      }
  }


}

float ping_getDistance (void){
    ping_trigger();

    //Use dir to switch to input
    GPIO_PORTB_DIR_R &= 0b00000000;

    float time_diff;
    int overflow = (START_TIME < END_TIME);
    while(STATE != DONE){};
        if(START_TIME > END_TIME){
            time_diff = START_TIME - END_TIME;
            lcd_printf("%d %d", time_diff, count);
        }
        else{
            time_diff = ((unsigned long)overflow << 24) + START_TIME - END_TIME;
            count++;
            lcd_printf("%d %d", time_diff, count);
        }
        time_diff /= 16000000;
        time_diff = (time_diff / 2) * 343 * 100;
        lcd_printf("%.2f %d", time_diff, count);
        return time_diff;
}
