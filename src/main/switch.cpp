#include "switch.h"

const uint8_t SWITCH_PIN = 9;
const uint8_t LED_GREEN_PIN = 8;
const uint8_t LED_RED_PIN = 2;

void switchSetup() {
  // Initialize the switch and LED pins
  pinMode(SWITCH_PIN, INPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  Serial.println(F("Switch and LEDs setup finished\n"));
}

int getSwitchState() {
  // Read the state of the switch
  return digitalRead(SWITCH_PIN);
}

void greenLedOn() {
  // Turn on the green LED and turn off the red LED
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
}

void redLedOn() {
  // Turn on the red LED and turn off the green LED
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
}
