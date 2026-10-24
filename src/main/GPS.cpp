#include <Arduino.h>
#include <Adafruit_GPS.h>

// Wired on hardware Serial1 (see kicad-testing-station schematic:
// GPS TX -> RX1, GPS RX -> TX1)
#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

void GPSSetup() {
  Serial.println(F("GPS setup start"));

  // 9600 NMEA is the default baud rate for the Adafruit Ultimate GPS
  GPS.begin(9600);

  // Turn on RMC (recommended minimum) and GGA (fix data incl. altitude)
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // 1 Hz update rate: the parser needs time to sort through the data,
  // don't go higher than this
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

  // Request antenna status updates
  GPS.sendCommand(PGCMD_ANTENNA);

  Serial.println(F("GPS setup finished\n"));
}

// Temporary diagnostic: echo raw bytes received from the GPS to the
// USB Serial console, so we can see whether the module is transmitting
// anything at all (as opposed to transmitting but never getting a fix).
// Set to false once wiring/communication is confirmed working.
#define GPS_DEBUG_ECHO false

void GPSRead() {
  // Pull in whatever bytes are waiting on the serial buffer. This has
  // to run every loop() iteration or characters get dropped and the
  // parser never sees a complete sentence.
  char c = GPS.read();

  if (GPS_DEBUG_ECHO && c) {
    Serial.print(c);
  }

  if (GPS.newNMEAreceived()) {
    // parse() also clears the newNMEAreceived() flag; if parsing fails
    // we just wait for the next sentence
    GPS.parse(GPS.lastNMEA());
  }
}

bool getGPSFix() {
  return GPS.fix;
}

float getGPSLatitude() {
  if (!GPS.fix) {
    return 0.0;
  }
  return GPS.latitudeDegrees;
}

float getGPSLongitude() {
  if (!GPS.fix) {
    return 0.0;
  }
  return GPS.longitudeDegrees;
}

float getGPSAltitude() {
  if (!GPS.fix) {
    return 0.0;
  }
  return GPS.altitude; // meters
}

uint8_t getGPSSatellites() {
  return GPS.satellites;
}