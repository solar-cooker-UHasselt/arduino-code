#include <Arduino.h>
#include "Pt100.h"
#include "pyranometer.h"
#include "switch.h"
#include <Wire.h>

bool debug = true;

unsigned long testDurationMillis = 0;

int stopMessagePrinted = 0;

unsigned long timestamp;
unsigned long previousMillisSD;

void microSDSetup();
void writeCSVHeaders();
void writeDataToSD();
void updateFileName();
void makeFile();
void writeLine(int line, const char *format, float value, float &previousValue);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  Serial.println();

  //microSDSetup();
  Pt100Setup();
  pyranoSetup();
  switchSetup();
}

void loop() {
  if (getSwitchState()) {
    greenLedOn();

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillisSD >= 1000) {
      writeDataToSD();
      previousMillisSD = currentMillis;
    }

    stopMessagePrinted = 0;

  } else {
    redLedOn();

    if(!stopMessagePrinted) {
        Serial.println("Testing stopped!\n");
        stopMessagePrinted = 1;
    }
  }
}

void writeDataToSD() {
  char dataSd[500];

  // Read PT100 temperatures and check for faults
  float temp1 = getPt100Temp1();
  float temp2 = getPt100Temp2();
  float temp3 = getPt100Temp3();
  bool fault1 = getPt100Fault_1();
  bool fault2 = getPt100Fault_2();
  bool fault3 = getPt100Fault_3();

  timestamp = millis();

  snprintf(dataSd, sizeof(dataSd), "%d;%s;%s;%s;%.2f", timestamp,
           fault1 ? "x" : String(temp1, 2).c_str(),
           fault2 ? "x" : String(temp2, 2).c_str(),
           fault3 ? "x" : String(temp3, 2).c_str(),
           getSolarIrradiance());

  Serial.println(dataSd);

  if (debug) {
    Serial.println(F("----------------"));
    Serial.print("Timestamp: ");
    Serial.println(timestamp);
    Serial.print("Temperature inside pot 1: ");

    if(!getPt100Fault_1()) {
      Serial.print(getPt100Temp1());
    } else {
      Serial.print("x");
    }
    Serial.println(" °C");

    Serial.print("Temperature inside pot 2: ");

    if(!getPt100Fault_2()) {
      Serial.print(getPt100Temp2());
    } else {
      Serial.print("x");
    }
    Serial.println(" °C");

    Serial.print("Temperature inside pot 3: ");

    if(!getPt100Fault_3()) {
      Serial.print(getPt100Temp3());
    } else {
      Serial.print("x");
    }
    Serial.println(" °C");

    Serial.print("Solar irradiance: ");
    Serial.print(getSolarIrradiance());
    Serial.println(" W/m2");

    Serial.println(F("CSV data"));
    Serial.println(dataSd);
    Serial.println(F("\n"));
  }
}
