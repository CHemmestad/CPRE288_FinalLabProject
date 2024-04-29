#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"

double moveForward(oi_t *sensor_data, double distance_mm);
double moveBack(oi_t *sensor_data, double distance_mm);
double turnRight(oi_t *sensor, double degrees);
double turnLeft(oi_t *sensor, double degrees);
void bump(oi_t *sensor);
double moveForwardWithDetect(oi_t *sensor_data, double distance_mm);

#endif
