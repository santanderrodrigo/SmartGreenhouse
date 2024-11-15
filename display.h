#ifndef DISPLAY_H
#define DISPLAY_H

// Clase base Display
// Esta clase define la interfaz para mostrar la temperatura y la humedad en una pantalla
class Display {
public:
  // Definimos los métodos virtuales que deben ser implementados por las clases derivadas
  virtual void begin() = 0;
  virtual void showTemperature(float temperature) = 0;
  virtual void showHumidity(float humidity) = 0;
  virtual void showMessage(int cursor, const char* message) = 0; 
  virtual void clear() = 0;
};

#endif