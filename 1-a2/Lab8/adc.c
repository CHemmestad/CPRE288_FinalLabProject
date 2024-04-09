/*
 * adc.c
 *
 *  Created on: Mar 19, 2024
 *      Author: kadenb
 */

#include "adc.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>

void adc_init(){

    //GPIO PORTB Initialization
    SYSCTL_RCGCGPIO_R |= 0b000010;
    GPIO_PORTB_AFSEL_R |= 0xFF;
    GPIO_PORTB_DIR_R |= 0b00010000;
    GPIO_PORTB_DEN_R &= 0b00000000;
    GPIO_PORTB_AMSEL_R |= 0b00010000;
    GPIO_PORTB_ADCCTL_R &= 0x00;

    //ADC 1 Initialization

    //Turn on ADC1
    SYSCTL_RCGCADC_R |= 0b10;

    //Busy wait for the adc to turn on
    while ((SYSCTL_PRADC_R & 0b10) == 0) {};

    //Turn on SS3
    ADC1_ACTSS_R &= 0b0000; // DISABLE

    ADC1_EMUX_R |= 0x0000;

    //ADCSSMUXn
    ADC1_SSMUX3_R = 0xA;

    ADC1_SSCTL3_R |= 0b0010;

    ADC1_SAC_R |= 0x4;

    ///ADCACTSS ENABLE
    ADC1_ACTSS_R |= 0b1000;

}

    uint16_t adc_read(){

        ADC1_PSSI_R |= 0b1000;

        //while((ADC1_RIS_R &= 0b1000)==0){};

        uint16_t adcResult = ADC1_SSFIFO3_R;

        ADC1_ISC_R = 0x0;


        return adcResult;
    }



