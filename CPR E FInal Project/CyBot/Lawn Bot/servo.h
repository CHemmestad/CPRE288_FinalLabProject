#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"
#include "button.h"


void servo_init(void);


void servo_move(uint16_t degrees);

void servo_calibrate();

#endif /* SERVO_H_ */
