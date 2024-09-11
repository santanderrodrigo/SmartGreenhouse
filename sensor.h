#ifndef SENSOR_H
#define SENSOR_H

// Clase base Sensor
// Esta clase define la interfaz para leer la temperatura y la humedad
class Sensor {
public:
  virtual void begin() = 0;
  virtual float readTemperature() = 0;
  virtual float readHumidity() = 0;
};

#endif