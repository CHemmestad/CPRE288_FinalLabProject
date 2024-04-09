/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */
#include <string.h>

#include "Timer.h"
#include "lcd.h"

void copyString(char string1[], char string2[]);
void shiftString(char string[]);

int main (void) {

    char sentence[] = "Microcontrollers are lots of fun! ";
    char sentence2[21];

	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)
	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 

	// Print "Hello, world" on the LCD
	//lcd_puts("Hello, world");

	while(1) {
        timer_waitMillis(300);

        lcd_clear();

        copyString(sentence, sentence2);

        lcd_puts(sentence2);

        shiftString(sentence);
	}

	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}

void shiftString(char string[]) {
    char firstChar = string[0];
    int i = 0;
    for(; i < strlen(string); i++) {
        string[i] = string[i+1];
    }
    string[strlen(string)] = firstChar;
}

void copyString(char string1[], char string2[]) {
    strncpy(string2, string1, 19);
    string2[20] = '\0';
}
