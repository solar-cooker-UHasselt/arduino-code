#include <Arduino.h>

static const uint8_t pyranometerPin = 10; // Analog input pin used for pyranometer

void setup() {
  Serial.begin(115200); // Start serial communication at 9600 bps
  pinMode(pyranometerPin, INPUT);
}

void loop() {
  int pyranometerValue = analogRead(pyranometerPin); // Read pyranometer value
  float pyranometerVoltage =
      pyranometerValue * (5.0 / 1023.0); // Convert pyranometer value to voltage
                                         // (assuming 5V reference voltage)
  float pyranometerIrradiance =
      pyranometerVoltage *
      400; // Convert pyranometer voltage to irradiance calibration factor (0.4 W/m2 per mV)

  Serial.print("Pin A5: ");
  Serial.println(pyranometerValue);
  Serial.print("Pyranometer voltage: ");
  Serial.print(pyranometerVoltage, 3);
  Serial.print(" V | ");
  Serial.print("Pyranometer irradiance: ");
  Serial.print(pyranometerIrradiance, 2);
  Serial.println(" W/m^2");

  delay(1000); // Wait 1 second before taking the next reading
}
