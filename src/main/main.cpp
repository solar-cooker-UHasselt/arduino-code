#include <Arduino.h>
#include "anemometer.h"
#include "AM2315C.h"
#include "BME680.h"
#include "clock.h"
#include "Pt100.h"
#include "pyranometer.h"
#include "switch.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SPI.h"
#include <SdFat.h>

bool debug = true;

#define TFT_DC 7
#define TFT_CS 3
#define TFT_RST 5

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

const int CS_SD = 4;

SdFat sd;
SdFile myFile;

char filePath[200];
int fileNameUpdated = 0;
unsigned long previousMillisSD = 0;
unsigned long previousMillisScreen = 0;
unsigned long testDurationMillis = 0;

float tempBuffer[60];
int bufferIndex = 0;
int isArrayEmpty = 1;

char previousLine[10][100] = {""};
float previousValues[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

void LCDSetup();
void writeDataToScreen();
void microSDSetup();
void writeCSVHeaders();
void writeDataToSD();
void updateFileName();
void makeFile();
void shiftAndAppend(float array[], float value);
void initArray(float array[], float value);
void writeLine(int line, const char *format, float value, float &previousValue);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  Serial.println();

  AM2315CSetup();
  anemoSetup();
  BME680Setup();
  clockSetup();
  microSDSetup();
  Pt100Setup();
  pyranoSetup();
  switchSetup();
  LCDSetup();
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

    if (currentMillis - previousMillisScreen >= 1000) {
      writeDataToScreen();
      previousMillisScreen = currentMillis;
    }

    if (currentMillis - previousMillisSD >= 10000) {
      writeDataToSD();
      previousMillisSD = currentMillis;
    }
  } else {
    redLedOn();
    fileNameUpdated = 0;
    isArrayEmpty = 1;
  }
}

void microSDSetup() {
  Serial.println(F("microSD setup start"));
  if (!sd.begin(CS_SD, SPI_FULL_SPEED)) {
    sd.initErrorHalt();
  }
  Serial.println(F("microSD setup finished\n"));
}

void writeCSVHeaders() {
  const char CSVHeaders[] =
    "Year;Month;Day;Hour;Minute;Second;Outside temperature "
    "[°C];Wind speed [m/s];Air pressure (inside box) [hPa];Relative humidity "
    "(inside box) [%];Temperature inside pot 1 [°C];Temperature inside pot 2 "
    "[°C];Temperature inside pot 3 [°C];Solar irradiance [W/m²]";
  if (!myFile.open("test123.csv", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt(F("opening test.txt for write failed"));
  }
  Serial.println("Writing CSV headers");
  myFile.println(CSVHeaders);
  myFile.close();
  Serial.println(F("done writing.\n"));
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

  snprintf(dataSd, sizeof(dataSd), "%04d;%02d;%02d;%02d;%02d;%02d;%.2f;%.2f;%.2f;%.2f;%s;%s;%s;%.2f",
           getYear(), getMonth(), getDay(), getHour24(), getMinute(), getSecond(),
           getAM2315CTemp(), getWindSpeed(), getBME680Pressure(), getAM2315CHum(),
           fault1 ? "x" : String(temp1, 2).c_str(),
           fault2 ? "x" : String(temp2, 2).c_str(),
           fault3 ? "x" : String(temp3, 2).c_str(),
           getSolarIrradiance());

  if (!myFile.open("test123.csv", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt(F("opening test.txt for write failed"));
  }
  myFile.println(dataSd);
  myFile.close();

  if (debug) {
    Serial.println(F("----------------"));
    Serial.print("Date: ");
    Serial.print(getYear());
    Serial.print("-");
    Serial.print(getMonth());
    Serial.print("-");
    Serial.println(getDay());

    Serial.print("Time: ");
    Serial.print(getHour24());
    Serial.print(":");
    Serial.print(getMinute());
    Serial.print(":");
    Serial.println(getSecond());

    Serial.print("Outside temperature: ");
    Serial.print(getAM2315CTemp());
    Serial.println(" °C");

    Serial.print("Windspeed: ");
    Serial.print(getWindSpeed());
    Serial.println(" m/s");

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

    Serial.println(F("\nCSV data"));
    Serial.println(dataSd);
    Serial.println(F("\n"));
  }
}

void updateFileName() {
  snprintf(filePath, sizeof(filePath), "%04d/%02d/%02d%02d%02d%02d.csv", getYear(),
           getMonth(), getDay(), getHour24(), getMinute(), getSecond());

  Serial.println(filePath);

  char dirName[200];
  snprintf(dirName, sizeof(dirName), "%04d/%02d/", getYear(), getMonth());

  char yearDir[10];
  snprintf(yearDir, sizeof(yearDir), "%04d", getYear());
  sd.mkdir(yearDir);
  sd.mkdir(dirName);

  makeFile();
}

void makeFile() {
  if (!myFile.open("test123.csv", O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening file for write failed");
  }
  myFile.close();
}

void LCDSetup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
}

void writeDataToScreen() {
  unsigned long durationMillis = millis() - testDurationMillis;
  float hours = durationMillis / 3600000;
  float minutes = (durationMillis % 3600000) / 60000;
  float seconds = (durationMillis % 60000) / 1000;

  char timeStr[100];
  snprintf(timeStr, sizeof(timeStr), "Time  : %02dh %02dm %02ds", (int)hours, (int)minutes, (int)seconds);
  writeLine(0, timeStr, 0, previousValues[0]);  // Passing 0 as a dummy value for float

  if (!getPt100Fault_1())
    writeLine(1, "Pot 1 : %.2f C", getPt100Temp1(), previousValues[1]);
  else
    writeLine(1, "Pot 1 : NULL", -1, previousValues[1]);

  if (!getPt100Fault_2())
    writeLine(2, "Pot 2 : %.2f C", getPt100Temp2(), previousValues[2]);
  else
    writeLine(2, "Pot 2 : NULL", -1, previousValues[2]);

  if (!getPt100Fault_3())
    writeLine(3, "Pot 3 : %.2f C", getPt100Temp3(), previousValues[3]);
  else
    writeLine(3, "Pot 3 : NULL", -1, previousValues[3]);

  writeLine(4, "Wind  : %.2f m/s", getWindSpeed(), previousValues[4]);
  writeLine(5, "Irr : %.2f W/m2", getSolarIrradiance(), previousValues[5]);

  if (!getPt100Fault_1()) {
    float temp = getPt100Temp1();
    if (isArrayEmpty) {
      initArray(tempBuffer, temp);
      isArrayEmpty = 0;
    } else {
      shiftAndAppend(tempBuffer, temp);
    }
  }

  float slopeFloat = tempBuffer[59] - tempBuffer[0];
  writeLine(6, "Slope 1min: %.2fC", slopeFloat, previousValues[6]);

  writeLine(7, "Temp out: %.2fC", getAM2315CTemp(), previousValues[7]);
  writeLine(8, "Humidity: %.2f%%", getAM2315CHum(), previousValues[8]);
  writeLine(9, filePath, 0, previousValues[9]);  // Passing 0 as a dummy value for float
}

void writeLine(int line, const char *format, float value, float &previousValue) {
  if (previousValue != value) {
    previousValue = value;
    tft.setCursor(0, line * 20);
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.println(previousLine[line]);
    snprintf(previousLine[line], sizeof(previousLine[line]), format, value);
    tft.setCursor(0, line * 20);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println(previousLine[line]);
  }
}

void shiftAndAppend(float array[], float value) {
  memmove(array, array + 1, (59 * sizeof(float)));
  array[59] = value;
}

void initArray(float array[], float value) {
  for (size_t i = 0; i < 60; i++) {
    array[i] = value;
  }
}
