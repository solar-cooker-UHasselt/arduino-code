#ifndef CLOCK_H
#define CLOCK_H

#include <RTClib.h>

extern RTC_DS3231 rtc;

void clockSetup(bool updateTimeFromPC);
uint16_t getYear(const DateTime &now);
uint8_t getMonth(const DateTime &now);
uint8_t getDay(const DateTime &now);
uint8_t getHour24(const DateTime &now);
uint8_t getMinute(const DateTime &now);
uint8_t getSecond(const DateTime &now);
DateTime getTime();

#endif
