#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "button.h"

unsigned long pwm_period = 0x4E200;

void servo_init(void);


uint16_t servo_move(uint16_t degrees);

void servo_calibrate();

#endif /* SERVO_H_ */
