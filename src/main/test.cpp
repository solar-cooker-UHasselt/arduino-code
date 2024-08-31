#include <Arduino.h>
#include "clock.h"
#include "Pt100.h"
#include "pyranometer.h"
#include "switch.h"
#include <Wire.h>
#include <SdFat.h>

bool debug = true;
bool updateTimeFromPC = false;

const int CS_SD = 4;

SdFat sd;
SdFile myFile;

char filePath[200];
int fileNameUpdated = 0;
unsigned long timestamp;
unsigned long previousMillisSD = 0;
unsigned long testDurationMillis = 0;

int stopMessagePrinted = 0;

void microSDSetup();
void writeCSVHeaders();
void writeDataToSD();
void updateFileName();
void makeFile();

void setup() {
  Serial.begin(115200);
  while (!Serial) { }
  Serial.println();

  clockSetup(updateTimeFromPC);
  microSDSetup();
  Pt100Setup();
  pyranoSetup();
  switchSetup();
}

void loop() {
  if (getSwitchState()) {
    greenLedOn();

    if (!fileNameUpdated) {
      testDurationMillis = millis();
      updateFileName();
      writeCSVHeaders();
      fileNameUpdated = 1;
    }

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

void microSDSetup() {
  Serial.println(F("MicroSD setup start"));
  if (!sd.begin(CS_SD, SPI_FULL_SPEED)) {
    sd.initErrorHalt();
  }
  Serial.println(F("MicroSD setup finished\n"));
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

void writeCSVHeaders() {
  const char CSVHeaders[] =
    "Timestamp;Temperature inside pot 1 [°C];Temperature inside pot 2 "
    "[°C];Temperature inside pot 3 [°C];Solar irradiance [W/m²]";
  if (!myFile.open(filePath, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt(F("opening file for write failed"));
  }
  Serial.println("Writing CSV headers");
  myFile.println(CSVHeaders);
  myFile.close();
  Serial.println(F("Done writing.\n"));
}

void updateFileName() {
  DateTime now = getTime();
  snprintf(filePath, sizeof(filePath), "%04d/%02d/%02d%02d%02d%02d.csv", getYear(now),
           getMonth(now), getDay(now), getHour24(now), getMinute(now), getSecond(now));

  Serial.print("CSV path name: ");
  Serial.println(filePath);

  char dirName[200];
  snprintf(dirName, sizeof(dirName), "%04d/%02d/", getYear(now), getMonth(now));

  char yearDir[10];
  snprintf(yearDir, sizeof(yearDir), "%04d", getYear(now));
  sd.mkdir(yearDir);
  sd.mkdir(dirName);

  makeFile();
}

void makeFile() {
  if (!myFile.open(filePath, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening file for write failed");
  }
  myFile.close();
}
