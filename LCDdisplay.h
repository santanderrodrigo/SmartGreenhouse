#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include "Display.h"
#include <LiquidCrystal.h>

// Clase derivada LCDDisplay
// Implementa los métodos de la clase base Display para una pantalla LCD
class LCDDisplay : public Display {
public:
  LCDDisplay(int rs, int enable, int d4, int d5, int d6, int d7);
  void begin() override;
  void showTemperature(float temperature) override;
  void showHumidity(float humidity) override;
private:
  LiquidCrystal lcd;
};

#endif