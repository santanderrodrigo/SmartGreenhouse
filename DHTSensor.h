#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <DHT.h> // Incluimos la librería del sensor DHT
#include "Sensor.h" // Incluimos la clase base Sensor

class DHTSensor : public Sensor {
private:
  DHT dht;
public:
  DHTSensor(uint8_t pin, uint8_t type) : dht(pin, type) {}
  void begin() override {
    dht.begin();
  }
  float readTemperature() override {
    return dht.readTemperature();
  }
  float readHumidity() override {
    return dht.readHumidity();
  }
};

#endif