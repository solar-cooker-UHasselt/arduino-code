#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include "RTClib.h"

extern RTC_DS3231 rtc;

void clockSetup();
uint16_t getYear();
uint8_t getMonth();
uint8_t getDay();
uint8_t getHour24();
uint8_t getMinute();
uint8_t getSecond();

#endif
