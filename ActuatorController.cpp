#include "ActuatorController.h"

ActuatorController::ActuatorController(uint8_t fan, uint8_t pump1, uint8_t pump2, uint8_t pump3, uint8_t sprayer)
  : fanPin(fan), pump1Pin(pump1), pump2Pin(pump2), pump3Pin(pump3), sprayerPin(sprayer),
    fanState(false), pump1State(false), pump2State(false), pump3State(false), sprayerState(false) {}

void ActuatorController::begin() {
  pinMode(fanPin, OUTPUT);
  pinMode(pump1Pin, OUTPUT);
  pinMode(pump2Pin, OUTPUT);
  pinMode(pump3Pin, OUTPUT);
  pinMode(sprayerPin, OUTPUT);

  // Aseguramos que todos los actuadores estén apagados al inicio
  turnOff(FAN);
  turnOff(PUMP1);
  turnOff(PUMP2);
  turnOff(PUMP3);
  turnOff(SPRAYER);
}

void ActuatorController::setActuatorState(Actuator actuator, bool state) {
  uint8_t pin;
  bool* actuatorState;

  // Seleccionamos el pin y el estado del actuador correspondiente
  switch (actuator) {
    case FAN: pin = fanPin; actuatorState = &fanState; break;
    case PUMP1: pin = pump1Pin; actuatorState = &pump1State; break;
    case PUMP2: pin = pump2Pin; actuatorState = &pump2State; break;
    case PUMP3: pin = pump3Pin; actuatorState = &pump3State; break;
    case SPRAYER: pin = sprayerPin; actuatorState = &sprayerState; break;
  }

  // Establecemos el estado del pin y guardamos el estado actual
  digitalWrite(pin, state ? HIGH : LOW);
  *actuatorState = state;
}

void ActuatorController::turnOn(Actuator actuator) {
  setActuatorState(actuator, true);
}

void ActuatorController::turnOff(Actuator actuator) {
  setActuatorState(actuator, false);
}

bool ActuatorController::isOn(Actuator actuator) {
  switch (actuator) {
    case FAN: return fanState;
    case PUMP1: return pump1State;
    case PUMP2: return pump2State;
    case PUMP3: return pump3State;
    case SPRAYER: return sprayerState;
    default: return false;
  }
}
