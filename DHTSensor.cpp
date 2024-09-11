#include "DHTSensor.h"

// Constructor de la clase DHTSensor
DHTSensor::DHTSensor(uint8_t pin, uint8_t type) : dht(pin, type) {}

// Implementación del método begin
void DHTSensor::begin() {
  dht.begin(); // Inicializamos el sensor DHT
}

// Implementación del método readTemperature
float DHTSensor::readTemperature() {
  return dht.readTemperature(); // Leemos la temperatura
}

// Implementación del método readHumidity
float DHTSensor::readHumidity() {
  return dht.readHumidity(); // Leemos la humedad
}