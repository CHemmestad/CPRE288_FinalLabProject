
#include "Timer.h"
#include "lcd.h"
#include "adc.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>


/**
 * main.c
 */
int main(void)
{

    timer_init();
    lcd_init();
    adc_init();

    char message[50];
    uint16_t adcValue;
    float calculatedValue = 0;
    while(true){

        adcValue = adc_read();
        calculatedValue = 8277.2 * pow(adcValue, -0.903);

        sprintf(message, "ADC: %u           CM: %.2f", adcValue, calculatedValue);


        lcd_puts(message);

        timer_waitMillis(200);
        lcd_clear();
    }




	return 0;
}
