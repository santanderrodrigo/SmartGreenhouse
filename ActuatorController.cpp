#include "ActuatorController.h"

ActuatorController::ActuatorController(uint8_t fan, uint8_t pump1, uint8_t pump2, uint8_t pump3, uint8_t sprayer)
  : fanPin(fan), pump1Pin(pump1), pump2Pin(pump2), pump3Pin(pump3), sprayerPin(sprayer) {}

void ActuatorController::begin() {
  pinMode(fanPin, OUTPUT);
  pinMode(pump1Pin, OUTPUT);
  pinMode(pump2Pin, OUTPUT);
  pinMode(pump3Pin, OUTPUT);
  pinMode(sprayerPin, OUTPUT);
}

void ActuatorController::turnFanOn() {
  digitalWrite(fanPin, HIGH);
}

void ActuatorController::turnFanOff() {
  digitalWrite(fanPin, LOW);
}

void ActuatorController::turnPump1On() {
  digitalWrite(pump1Pin, HIGH);
}

void ActuatorController::turnPump1Off() {
  digitalWrite(pump1Pin, LOW);
}

void ActuatorController::turnPump2On() {
  digitalWrite(pump2Pin, HIGH);
}

void ActuatorController::turnPump2Off() {
  digitalWrite(pump2Pin, LOW);
}

void ActuatorController::turnPump3On() {
  digitalWrite(pump3Pin, HIGH);
}

void ActuatorController::turnPump3Off() {
  digitalWrite(pump3Pin, LOW);
}

void ActuatorController::turnSprayerOn() {
  digitalWrite(sprayerPin, HIGH);
}

void ActuatorController::turnSprayerOff() {
  digitalWrite(sprayerPin, LOW);
}