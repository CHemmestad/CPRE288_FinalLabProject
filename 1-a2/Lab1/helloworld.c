/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"


void lcd_rotatingBanner();


int main (void) {

	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	//call the rotating banner function
	while(1){
	    lcd_rotatingBanner();
	    lcd_init();
	}
	// Print "Hello, world" on the LCD


	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}

void lcd_rotatingBanner() {

    char str[] = "Microcontrollers are lots of fun!";
    char tempStr[19] = "                   ";
    uint32_t millis = 300;
    int strLen = strlen(str);
    int len = 19;
    int i = 0;
    int letterCount = 0;
    while(letterCount <= 53) {
        timer_waitMillis(millis);

        lcd_printf(tempStr);

//        int i;
//        for(i = 0; i < strLen -1; i++){
//            str[i]
//        }

            int z;
        for(z = 0; z < 19; z++) {
            tempStr[z] = tempStr[z+1];
        }
        tempStr[len] = str[i];
//
//        len--;
        i++;
        letterCount++;
    }

}
