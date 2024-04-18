/*
CPR E 288 Final Project
Title: Lawn Bot

Contributors:
Caleb Hemmestad
Jerry liu
Billy Dang
Alex Svobodny
Kaden Berger

Date: 04/18/24

Description: Making a Lawn Bot mow the entire lawn without hitting any objects or getting stuck 
and then having it finish by going to the final destination

Last Updated: 04/18/24
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "button.h"
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1
#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors
#include "open_interface.h"
#include "movement.h"
#include "driverlib/interrupt.h"
#include "uart-interrupt.h"
#include "adc.h"
#include "ping_template.h"
#include "servo.h"
#include "AdditionalFunctions.h"

//#warning "Possible unimplemented functions"
#define REPLACEME 0

void main(void) {
    cyBot_uart_init_clean();
    cyBot_uart_init();
    cyBOT_init_Scan(0b111);
    timer_init();
	lcd_init();
    //To Do: Include the rest of the inits()

	cyBot_uart_init_clean();
	servo_init2();
    //To Do: Include the inits() for the ones we created

    //To Do: Make the Lawn Bot do stuff
}
