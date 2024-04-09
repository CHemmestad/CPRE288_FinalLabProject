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

//#include "uart.h"

#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "open_interface.h"
#include "movement.h"

#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "uart-interrupt.h"


//#warning "Possible unimplemented functions"
#define REPLACEME 0

void printString2(char string[]) {
    int j;
    for(j = 0; j < strlen(string); j++) {
        lcd_putc(string[j]);
    }
}


void main(void) {
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();
    timer_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0111);
    //cyBOT_SERVO_cal();
    right_calibration_value = 285250;
    left_calibration_value = 1246000;
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();

  // initialize the cyBot UART1 before trying to use it

  // (Uncomment ME for UART init part of lab)
	cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code

	uart_interrupt_init();
	cyBOT_init_Scan(0b0111);

	cyBOT_Scan_t* data = calloc(1, sizeof(cyBOT_Scan_t));
	cyBOT_Scan(90, data);
	int i = 0;
	char go;
	while(1) {
	    i = 0;
        for(; i < 180; i++) {
//    	    char received = uart_receive_nonblocking();
//    	    if(received == 's') {
//    	        //uart_sendChar(received);
//    	        break;
//    	    }
            while(go != 'g') {
                go = uart_receive();
            }
            if(command_flag == 1) {
                command_flag = 0;
                go = NULL;
            } else if(command_flag == 2) {
                command_flag = 0;
                go = NULL;
                cyBOT_Scan(90, data);
                break;
            }
            //uart_sendChar(received);
            cyBOT_Scan(i, data);
        }
	}
	//cyBOT_Scan(90, data);

//	uart_sendChar('A');
//
//	lcd_putc(uart_receive());
//
//	char string[21];
////	uart_sendStr(string);
//
//	lcd_clear();
//
////	printString2(string);
////	printString(string);
//
//	// YOUR CODE HERE
//
//	while(1)
//	{
//
//	    uart_sendStr(string);
//
//        lcd_clear();
//
//        printString2(string);
//        printString(string);
//      // YOUR CODE HERE
//
//
//	}

}
