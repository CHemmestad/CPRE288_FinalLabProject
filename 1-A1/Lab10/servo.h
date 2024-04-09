/*
 * servo.h
 *
 *  Created on: Apr 2, 2024
 *      Author: cihem
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void servo_init2(void);

void servo_move2(uint16_t degrees);

#endif /* SERVO_H_ */
