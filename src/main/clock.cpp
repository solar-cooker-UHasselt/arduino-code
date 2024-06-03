#include <Arduino.h>

#include "RTClib.h"
#include <SPI.h>

RTC_DS3231 rtc;

void clockSetup() {
  Serial.println(F("clock setup start"));
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now();
  Serial.print("Date: ");
  Serial.println(F(__DATE__));
  Serial.print("Time: ");
  Serial.println(F(__TIME__));

  // if (rtc.lostPower()) {
  //   Serial.println("RTC lost power, let's set the time!");
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // }

  Serial.println(F("clock setup finished\n"));
}

uint16_t getYear() {
  DateTime now = rtc.now();
  return now.year();
}

uint8_t getMonth() {
  DateTime now = rtc.now();
  return now.month();
}

uint8_t getDay() {
  DateTime now = rtc.now();
  return now.day();
}

uint8_t getHour24() {
  DateTime now = rtc.now();
  return now.hour();
}

uint8_t getMinute() {
  DateTime now = rtc.now();
  return now.minute();
}

uint8_t getSecond() {
  DateTime now = rtc.now();
  return now.second();
}
