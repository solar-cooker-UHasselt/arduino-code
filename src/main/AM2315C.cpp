#include "AM2315C.h"

// Create an instance of the AHT10 sensor
Adafruit_AHTX0 aht;

void AM2315CSetup() {
  Serial.println(F("AM2315C setup start"));

  // Initialize the sensor
  if (!aht.begin()) {
    Serial.println(F("Could not find AM2315C, check wiring"));
    while (1) delay(10); // Halt the program if sensor is not found
  }

  Serial.println(F("AM2315C setup finished\n"));
}

float getAM2315CTemp() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  return temp.temperature;
}

float getAM2315CHum() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  return humidity.relative_humidity;
}
