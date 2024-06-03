#include <Arduino.h>

const uint8_t SWITCH_PIN = 9;
const uint8_t LED_GREEN_PIN = 8;
const uint8_t LED_RED_PIN = 2;

void switchSetup() {
  pinMode(SWITCH_PIN, INPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
}

int getSwitchState() { return digitalRead(SWITCH_PIN); }

void greenLedOn() {
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(LED_RED_PIN, LOW);
}

void redLedOn() {
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
}
