#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include "Display.h"
#include <LiquidCrystal.h>
#include <Arduino.h> // Incluimos la librería de Arduino para usar pinMode y digitalWrite

// Clase derivada LCDDisplay
// Implementa los métodos de la clase base Display para una pantalla LCD
class LCDDisplay : public Display {
private:
  int rwPin;
  LiquidCrystal lcd;

public:
  LCDDisplay(int rs, int rw, int enable, int d4, int d5, int d6, int d7);
  void begin() override;
  void showTemperature(float temperature) override;
  void showHumidity(float humidity) override;
  void showMessage(int x, int y, const char* message) override;
  void clear() override;
};

#endif