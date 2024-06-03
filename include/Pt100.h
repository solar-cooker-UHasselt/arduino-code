#ifndef PT100_H
#define PT100_H

#include <Arduino.h>
#include <Adafruit_MAX31865.h>

void Pt100Setup();
float getPt100Temp1();
float getPt100Temp2();
float getPt100Temp3();
uint8_t getPt100Fault_1();
uint8_t getPt100Fault_2();
uint8_t getPt100Fault_3();

#endif
