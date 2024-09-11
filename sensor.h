#ifndef SENSOR_H
#define SENSOR_H

// Clase base Sensor
// Esta clase define la interfaz para leer la temperatura y la humedad
class Sensor {
public:
  // Definimos los métodos virtuales que deben ser implementados por las clases derivadas
  virtual void begin() = 0; 
  virtual float readTemperature() = 0; 
  virtual float readHumidity() = 0;
};

#endif