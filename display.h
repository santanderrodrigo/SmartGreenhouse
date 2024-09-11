#ifndef DISPLAY_H
#define DISPLAY_H

// Clase base Display
// Esta clase define la interfaz para mostrar la temperatura y la humedad en una pantalla
class Display {
public:
  virtual void begin() = 0;
  virtual void showTemperature(float temperature) = 0;
  virtual void showHumidity(float humidity) = 0;
};

#endif