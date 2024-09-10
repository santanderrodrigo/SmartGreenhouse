#ifndef ACTUATORCONTROLLER_H
#define ACTUATORCONTROLLER_H

class ActuatorController {
private:
  uint8_t fanPin;
  uint8_t pumpPin;
  uint8_t greenLedPin;
  uint8_t redLedPin;

public:
  ActuatorController(uint8_t fan, uint8_t pump, uint8_t greenLed, uint8_t redLed)
    : fanPin(fan), pumpPin(pump), greenLedPin(greenLed), redLedPin(redLed) {}

  void begin() {
    pinMode(fanPin, OUTPUT);
    pinMode(pumpPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
  }

  void turnFanOn() {
    digitalWrite(fanPin, HIGH);
  }

  void turnFanOff() {
    digitalWrite(fanPin, LOW);
  }

  void turnPumpOn() {
    digitalWrite(pumpPin, HIGH);
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  }

  void turnPumpOff() {
    digitalWrite(pumpPin, LOW);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
  }
};

#endif