#include "pyranometer.h"

const int PYRANO_PIN = A5;

void pyranoSetup() {
  // Initialize the pyranometer sensor
  pinMode(PYRANO_PIN, INPUT);
  Serial.println(F("Pyranometer setup finished\n"));
}

float getSolarIrradiance() {
  // Read the analog value from the sensor
  int pyranoValue = analogRead(PYRANO_PIN);
  
  // Convert the analog value to voltage
  float pyranoVoltage = pyranoValue * (5.0 / 1023.0);
  
  // Convert the voltage to solar irradiance (W/m^2)
  // Assuming the sensor outputs 0-5V for 0-2000 W/m^2
  float pyranoIrradiance = pyranoVoltage * 400.0;
  
  return pyranoIrradiance;
}
