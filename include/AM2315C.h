#ifndef AM2315C_H
#define AM2315C_H

#include <Arduino.h>
#include <Adafruit_AHTX0.h>

void AM2315CSetup();
float getAM2315CTemp();
float getAM2315CHum();

#endif
