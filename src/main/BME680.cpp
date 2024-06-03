#include "BME680.h"

const uint8_t CS_BME680 = 6;
const float SEALEVELPRESSURE_HPA = 1013.25;

Adafruit_BME680 bme(CS_BME680);

void BME680Setup() {
  Serial.println(F("BME680 setup start"));
  
  // Initialize the BME680 sensor
  if (!bme.begin()) {
    Serial.println(F("Could not find BME680, check wiring"));
    while (1) delay(10); // Halt the program if sensor is not found
  }

  // Set sensor oversampling and filter settings
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320°C for 150 ms

  Serial.println(F("BME680 setup finished\n"));
}

float getBME680Humidity() {
  // Perform sensor reading
  if (!bme.performReading()) {
    Serial.println(F("BME680: Failed to perform reading :("));
    return -1.0; // Return error value
  }
  return bme.humidity; // Return humidity in %
}

float getBME680Pressure() {
  // Perform sensor reading
  if (!bme.performReading()) {
    Serial.println(F("BME680: Failed to perform reading :("));
    return -1.0; // Return error value
  }
  return bme.pressure / 100.0; // Return pressure in hPa
}

float getBME680Temperature() {
  // Perform sensor reading
  if (!bme.performReading()) {
    Serial.println(F("BME680: Failed to perform reading :("));
    return -1.0; // Return error value
  }
  return bme.temperature; // Return temperature in °C
}
