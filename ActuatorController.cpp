#include "ActuatorController.h"

// Constructor de la clase ActuatorController
ActuatorController::ActuatorController(uint8_t fan, uint8_t pump, uint8_t greenLed, uint8_t redLed)
  : fanPin(fan), pumpPin(pump), greenLedPin(greenLed), redLedPin(redLed) {}

// Implementación del método begin
void ActuatorController::begin() {
  pinMode(fanPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

   //dado que al iniciar la bomba esá apagada encendemos el LED rojo
   digitalWrite(redLedPin, HIGH);
}

// Implementación del método turnFanOn
void ActuatorController::turnFanOn() {
  digitalWrite(fanPin, HIGH);
}

// Implementación del método turnFanOff
void ActuatorController::turnFanOff() {
  digitalWrite(fanPin, LOW);
}

// Implementación del método turnPumpOn
void ActuatorController::turnPumpOn() {
  digitalWrite(pumpPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
}

// Implementación del método turnPumpOff
void ActuatorController::turnPumpOff() {
  digitalWrite(pumpPin, LOW);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
}