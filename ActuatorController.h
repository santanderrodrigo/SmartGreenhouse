#ifndef ACTUATORCONTROLLER_H
#define ACTUATORCONTROLLER_H

#include <Arduino.h> // Incluimos la librería de Arduino para usar pinMode y digitalWrite

class ActuatorController {
private:
  uint8_t fanPin;
  uint8_t pumpPin;
  uint8_t greenLedPin;
  uint8_t redLedPin;

public:
  ActuatorController(uint8_t fan, uint8_t pump, uint8_t greenLed, uint8_t redLed);
  void begin();
  void turnFanOn();
  void turnFanOff();
  void turnPumpOn();
  void turnPumpOff();
};

#endif