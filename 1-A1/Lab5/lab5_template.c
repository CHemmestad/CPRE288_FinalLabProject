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

#include "uart.h"

#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors


//#warning "Possible unimplemented functions"
#define REPLACEME 0

void printString2(char string[]) {
    int j;
    for(j = 0; j < strlen(string); j++) {
        lcd_putc(string[j]);
    }
}


int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();

  // initialize the cyBot UART1 before trying to use it

  // (Uncomment ME for UART init part of lab)
	cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code

	uart_init();

	uart_sendChar('A');

	lcd_putc(uart_receive());

	char string[21];
//	uart_sendStr(string);

	lcd_clear();

//	printString2(string);
//	printString(string);

	// YOUR CODE HERE

	while(1)
	{

	    uart_sendStr(string);

        lcd_clear();

        printString2(string);
        printString(string);
      // YOUR CODE HERE


	}

}
