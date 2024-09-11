#ifndef DHTSENSOR_H
#define DHTSENSOR_H

#include <DHT.h> // Incluimos la librería del sensor DHT
#include "Sensor.h" // Incluimos la clase base Sensor

// Clase DHTSensor, hereda de la clase Sensor que define la interfaz para leer la temperatura y la humedad
class DHTSensor : public Sensor {
private:
  DHT dht; // Creamos un objeto de la clase DHT
public:
  DHTSensor(uint8_t pin, uint8_t type) : dht(pin, type) {}
  void begin() override {
    dht.begin(); // Inicializamos el sensor DHT
  }
  float readTemperature() override {
    return dht.readTemperature(); // Leemos la temperatura
  }
  float readHumidity() override {
    return dht.readHumidity(); // Leemos la humedad
  }
};

#endif