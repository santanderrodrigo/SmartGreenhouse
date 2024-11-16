#ifndef ACTUATORCONTROLLER_H
#define ACTUATORCONTROLLER_H

#include <Arduino.h>

class ActuatorController {
public:
  ActuatorController(uint8_t fan, uint8_t pump1, uint8_t pump2, uint8_t pump3, uint8_t sprayer);
  void begin();
  void turnFanOn();
  void turnFanOff();
  void turnPump1On();
  void turnPump1Off();
  void turnPump2On();
  void turnPump2Off();
  void turnPump3On();
  void turnPump3Off();
  void turnSprayerOn();
  void turnSprayerOff();

private:
  uint8_t fanPin;
  uint8_t pump1Pin;
  uint8_t pump2Pin;
  uint8_t pump3Pin;
  uint8_t sprayerPin;
};

#endif