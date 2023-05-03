#include <Arduino.h>

float mapfloat(float x, float in_min, float in_max, float out_min,
               float out_max);

const int ANEMO_PIN = A0;

String getWindSpeed() {
  float sensorValue = analogRead(ANEMO_PIN);
  float voltage = (sensorValue / 1023) * 5;
  float windSpeed = mapfloat(voltage, 0.4, 2, 0, 32.4);
  return String(windSpeed);
}

float mapfloat(float x, float in_min, float in_max, float out_min,
               float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
