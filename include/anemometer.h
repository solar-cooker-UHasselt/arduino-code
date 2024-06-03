#ifndef ANEMOMETER_H
#define ANEMOMETER_H

#include <Arduino.h>

void anemoSetup();
float getWindSpeed();
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

#endif
