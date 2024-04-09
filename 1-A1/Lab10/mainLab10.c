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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "button.h"
#include "timer.h"
#include "lcd.h"
//#include "uart.h"
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

//#warning "Possible unimplemented functions"
#define REPLACEME 0

void printString2(char something[]) {
    int j;
    for(j = 0; j < strlen(something); j++) {
        lcd_putc(something[j]);
    }
}

void makeString(char* string, int number) {
    *(string + 7) = '\0';
    *(string + 6) = (char)((number % 10) + 48);
    *(string + 5) = (char)(((number / 10) % 10) + 48);
    *(string + 4) = (char)(((number / 100) % 10) + 48);
    *(string + 3) = (char)(((number / 1000) % 10) + 48);
    *(string + 2) = (char)(((number / 10000) % 10) + 48);
    *(string + 1) = (char)(((number / 100000) % 10) + 48);
    *(string + 0) = (char)(((number / 1000000) % 10) + 48);
}

int calculateDistance(int value) {
    int results = round(1.0 / ((value / 4095.0) * .13));
    if(results > 80) {
        return 80;
    } else {
        return results;
    }
}

int calculatePingDis() {
    double divide = RESULTS / 1.6E7;
    double mult = 343 * divide;
    double half = mult / 2;
    double cm = half * 100;
    return (int)cm;
}


void main(void) {
    cyBot_uart_init_clean();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0111);
    //cyBOT_SERVO_cal();
    right_calibration_value = 285250;
    left_calibration_value = 1246000;
    timer_init();
	lcd_init();

	cyBot_uart_init_clean();
	servo_init2();

	servo_move2(0);
	servo_move2(90);
	servo_move2(30);
	servo_move2(150);
	servo_move2(90);
















	//ping_init ();

//	int cm;
//	char* number = (char*)malloc(7 * sizeof(char));

//	while(1) {
//	    timer_waitMillis(500);
//	    lcd_clear();
//	    ping_trigger();
//	    makeString(number, RESULTS);
//	    lcd_puts(number);
//	    lcd_setCursorPos(0, 1);
//	    if(OVERFLOW) {
//	        lcd_puts("Overflow!");
//	        OVERFLOW = 0;
////	        int stop = 1;
////	        while(stop){
////	            scanf("%d", &stop);
////	        }
//	    } else {
//	        lcd_puts("No overflow");
//	    }
//	    lcd_setCursorPos(0, 3);
//	    cm = calculatePingDis();
//	    makeString(number, cm);
//	    lcd_puts(number);
//	    lcd_puts("cm");
//    }
}
