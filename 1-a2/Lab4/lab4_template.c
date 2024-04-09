/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "cyBot_uart.h"
#include "cyBot_Scan.h"
#include "Timer.h"
#include "lcd.h"
#include "open_interface.h"
#include "float.h"
#include "math.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1


#define REPLACEME 0

char button[] = "button";

int main(void) {
    button_init();
    timer_init();
    lcd_init();
    cyBot_uart_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

	lcd_clear();

	
	while(1)
	{
	    int i = 0;
	    int c = button_getButton();

	    cyBot_sendByte(c);

	    if(c == 4){
	        sprintf(button, "You pressed button %d\r\n", c);
	        lcd_clear();
	        lcd_printf("                   %d", c);
	        timer_waitMillis(250);
	    }
	    else if(c == 3){
	        sprintf(button, "You pressed button %d\r\n", c);
	        lcd_clear();
	        lcd_printf("              %d", c);
	        timer_waitMillis(250);
	    }
        else if(c == 2){
            sprintf(button, "You pressed button %d\r\n", c);
            lcd_clear();
            lcd_printf("       %d", c);
            timer_waitMillis(250);
        }
        else if(c == 1){
            sprintf(button, "You pressed button %d\r\n", c);
            lcd_clear();
            lcd_printf("%d", c);
            timer_waitMillis(250);
        }
	    if(c <= 4 && c >= 1){
	        for(i = 0; i < strlen(button); i++){
	            cyBot_sendByte(button[i]);
	        }
	    }
	    c = 0;
	}
	return 0;
}
