void GPSSetup();

// Must be called on every loop() iteration (not throttled) so no
// NMEA characters get dropped while waiting for a full sentence.
void GPSRead();

bool getGPSFix();

float getGPSLatitude();

float getGPSLongitude();

float getGPSAltitude();

uint8_t getGPSSatellites();
