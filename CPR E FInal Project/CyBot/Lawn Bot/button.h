/*
 * button.h
 *
 *  Created on: Jul 18, 2016
 *      Author: Eric Middleton
 *
 * @edit: Phillip Jones 05/30/2019 : Removed uneeded helper functions
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdint.h>
#include "cyBot_uart.h"
#include <inc/tm4c123gh6pm.h>

//initialize the push buttons
void printString(char string[]);
void button_init();


///Non-blocking call
///Returns highest value button being pressed, 0 if no button pressed
void button_getButton();


#endif /* BUTTON_H_ */
