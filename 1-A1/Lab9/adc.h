/*
 * adc.h
 *
 *  Created on: Mar 19, 2024
 *      Author: cihem
 */

#ifndef ADC_H_
#define ADC_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
//#include "uart.h"
#include "cyBot_uart.h"
#include <string.h>
#include <stdlib.h>

void adc_init(void);

int adc_read(void);

#endif /* ADC_H_ */
