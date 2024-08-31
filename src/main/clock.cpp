#include <Arduino.h>
#include <RTClib.h>
#include "clock.h"

RTC_DS3231 rtc;

void clockSetup(bool updateTimeFromPC) {
  Serial.println(F("Clock setup start"));
  if (!rtc.begin()) {
    Serial.println(F("Clock setup failed: Couldn't find RTC"));
    while (1) delay(10); // Halt the program if RTC is not found
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else if (updateTimeFromPC) {
    Serial.println(F("Setting the time!"));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  rtc.now();
  Serial.print(F("Current date: "));
  Serial.println(F(__DATE__));
  Serial.print(F("Current time: "));
  Serial.println(F(__TIME__));
  Serial.println(F("Clock setup finished\n"));
}

uint16_t getYear(const DateTime &now) {
  return now.year();
}

uint8_t getMonth(const DateTime &now) {
  return now.month();
}

uint8_t getDay(const DateTime &now) {
  return now.day();
}

uint8_t getHour24(const DateTime &now) {
  return now.hour();
}

uint8_t getMinute(const DateTime &now) {
  return now.minute();
}

uint8_t getSecond(const DateTime &now) {
  return now.second();
}

DateTime getTime() {
  return rtc.now();
}
