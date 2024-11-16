#ifndef SOILMOISTURESENSOR_H
#define SOILMOISTURESENSOR_H

#include <Arduino.h>
#include "Sensor.h" // Incluimos la clase base Sensor

class SoilMoistureSensor : public Sensor {
public:
  SoilMoistureSensor(uint8_t pin);
  void begin() override;
  float readValue() ;
  float readTemperature() override { return 0.0; } // Dummy implementation
  float readHumidity() override { return 0.0; } // Dummy implementation
  void calibrate(float dryValue, float wetValue);

private:
  uint8_t pin;
  float dryValue;
  float wetValue;
};

#endif