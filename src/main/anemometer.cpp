#include "anemometer.h"

const int ANEMO_PIN = A0;

// Function to map a float value from one range to another
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function to initialize the anemometer sensor
void anemoSetup() {
  pinMode(ANEMO_PIN, INPUT);
  Serial.println(F("Anemometer setup finished\n"));
}

// Function to get the wind speed from the anemometer sensor
float getWindSpeed() {
  // Read the analog value from the sensor
  float sensorValue = analogRead(ANEMO_PIN);
  
  // Convert the analog value to voltage
  float voltage = (sensorValue / 1023.0) * 5.0;
  
  // Map the voltage to wind speed (0 to 30 m/s for a 0 to 5V sensor)
  float windSpeed = mapfloat(voltage, 0, 5, 0, 30);
  
  return windSpeed;
}
