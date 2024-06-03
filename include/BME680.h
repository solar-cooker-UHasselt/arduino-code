#ifndef BME680_H
#define BME680_H

#include <Arduino.h>
#include "Adafruit_BME680.h"

void BME680Setup();
float getBME680Temperature();
float getBME680Pressure();
float getBME680Humidity();

#endif
