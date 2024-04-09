/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"

#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors

#include "uart.h"
#include "string.h"



//#warning "Possible unimplemented functions"
#define REPLACEME 0


void getMessage(char message);
void lcdBuffer(char input);

char buffer[20];
int index = 0;


int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();

  // initialize the cyBot UART1 before trying to use it

  // (Uncomment ME for UART init part of lab)
	 cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code

	// Complete this code for configuring the GPIO PORTB part of UART1 initialization (your UART1 GPIO init code)
	SYSCTL_RCGCGPIO_R |= 0x02;
	        while ((SYSCTL_PRGPIO_R & 02) == 0) {};
	          GPIO_PORTB_DEN_R |= 0x0F;
	          GPIO_PORTB_AFSEL_R |= 0x3;
	      GPIO_PORTB_PCTL_R &= 0x00000000;     // Force 0's in the desired locations
	      GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations
		 // Or see the notes for a coding alternative to assign a value to the PCTL field

    // (Uncomment ME for UART init part of lab)
		 cyBot_uart_init_last_half();  // Complete the UART device configuration

		// Initialize the scan
		 //cyBOT_init_Scan();
		// Remember servo calibration function and variables from Lab 3

	// YOUR CODE HERE
		 int input;


	while(1)
	{

	    input = uart_receive();

	    lcdBuffer(input);
	    if(index >= 20){
	        lcd_clear();
	        lcd_printf("Buffer is full!");
	    }
	}

}

void getMessage(char message){
    char tempMsg[100];

    sprintf(tempMsg, "Got a message %c \n\r", message);
    //printf("Got a %s", tempMsg);

    uart_sendStr(tempMsg);

}

void lcdBuffer(char input){

    if(input == '\r' || input == '\n'){
        uart_sendChar('\r');
        uart_sendChar('\n');
        return;
    }

    if(index < 20 && input != '\x0d'){
        if(input == '\x7f'){
            buffer[index - 1] = ' ';
            index--;
            if(index < 0){
                index = 0;
            }
            lcd_setCursorPos(0, index);
        }
        else{
            buffer[index] = input;
            index++;
        }
        lcd_clear();
        //lcd_gotoLine(1);
        lcd_printf(buffer);
        lcd_gotoLine(2);
        char num[2];
        sprintf(num, "%d", index);
        lcd_puts(num);

        uart_sendChar(input);
    }
    if(index == 20){
        lcd_clear();
        lcd_printf("full!");
    }
}

