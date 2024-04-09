/*
 * adc.c
 *
 *  Created on: Mar 19, 2024
 *      Author: cihem
 */
#include "adc.h"

void adc_init(void) {
    SYSCTL_RCGCADC_R |= 0x0001;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while ((SYSCTL_PRGPIO_R & 0x1A) == 0) {};
    GPIO_PORTB_DIR_R &= ~0x02;
    GPIO_PORTB_AFSEL_R |= 0x02;
    GPIO_PORTB_DEN_R &= ~0x02;
    GPIO_PORTB_AMSEL_R |= 0x02;
    ADC0_ACTSS_R |= 0x1;
    ADC0_EMUX_R &= ~0xF;
    ADC0_SSCTL0_R = 0x0006;
    ADC0_SAC_R |= 0x04; //16 averaging
}

int adc_read(void) {
    int result;
    ADC0_PSSI_R = 0x1;
    while((ADC0_RIS_R & 0x1) == 0){};
    result = ADC0_SSFIFO0_R & 0xFFF;
    ADC0_ISC_R = 0x1;
    return result;
}
