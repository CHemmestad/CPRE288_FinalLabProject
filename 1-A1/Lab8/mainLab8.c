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

//#warning "Possible unimplemented functions"
#define REPLACEME 0

void printString2(char something[]) {
    int j;
    for(j = 0; j < strlen(something); j++) {
        lcd_putc(something[j]);
    }
}

void makeString(char* string, int number) {
    *(string + 4) = '\0';
    *(string + 3) = (char)((number % 10) + 48);
    *(string + 2) = (char)(((number / 10) % 10) + 48);
    *(string + 1) = (char)(((number / 100) % 10) + 48);
    *(string + 0) = (char)(((number / 1000) % 10) + 48);
}

int calculateDistance(int value) {
    int results = round(1.0 / ((value / 4095.0) * .13));
    if(results > 80) {
        return 80;
    } else {
        return results;
    }
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
	adc_init();
	char* something = (char*)malloc(5 * sizeof(char));
	int value = adc_read();
    int distance = calculateDistance(value);
	makeString(something, value);
	printString2(something);

	cyBOT_Scan_t* newData = calloc(1, sizeof(cyBOT_Scan_t));
	cyBOT_Scan(90, newData);


	while(1) {
	    int start = 0;
        for(; start < 180; start+=2) {
            cyBOT_Scan(start, newData);
            timer_waitMillis(250);
            lcd_clear();
            lcd_puts("Averaged IR: ");
            value = adc_read();
            makeString(something, value);
            lcd_puts(something);
            lcd_setCursorPos(0, 2);
            lcd_puts("Distance is: ");
            distance = calculateDistance(value);
            makeString(something, distance);
            lcd_puts(something);
            lcd_puts("cm");
        }
        // for(; start > 0; start-=2) {
        //     cyBOT_Scan(start, newData);
        //     timer_waitMillis(250);
        //     lcd_clear();
        //     value = adc_read();
        //     makeString(something, value);
        //     lcd_puts(something);
        // }
	}

	free(newData);
	free(something);
}
