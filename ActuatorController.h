#ifndef ACTUATORCONTROLLER_H
#define ACTUATORCONTROLLER_H

#include <Arduino.h>

// Enumeración para los actuadores
enum Actuator {
  FAN,
  PUMP1,
  PUMP2,
  PUMP3,
  SPRAYER
};

class ActuatorController {
public:
  ActuatorController(uint8_t fanPin, uint8_t pump1Pin, uint8_t pump2Pin, uint8_t pump3Pin, uint8_t sprayerPin);
  void begin();

  // Funciones genéricas para controlar actuadores
  void turnOn(Actuator actuator);
  void turnOff(Actuator actuator);
  bool isOn(Actuator actuator);

private:
  // Función interna para cambiar el estado del actuador
  void setActuatorState(Actuator actuator, bool state);

  // Pines de actuadores
  uint8_t fanPin;
  uint8_t pump1Pin;
  uint8_t pump2Pin;
  uint8_t pump3Pin;
  uint8_t sprayerPin;

  // Estado actual de los actuadores
  bool fanState;
  bool pump1State;
  bool pump2State;
  bool pump3State;
  bool sprayerState;
};

#endif
