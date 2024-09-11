#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include <LiquidCrystal.h> // Incluimos la librería de la pantalla LCD
#include "Display.h" // Incluimos la clase base Display

class LCDDisplay : public Display {
private:
  LiquidCrystal lcd;
public:
  LCDDisplay(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    : lcd(rs, enable, d4, d5, d6, d7) {}
  
  void begin() override {
    lcd.begin(16, 2); // Inicializamos la pantalla LCD de 16x2
  }
  
  void showTemperature(float temperature) override {
    lcd.setCursor(0, 0); // Nos movemos al inicio de la primera fila
    lcd.print("Temp: "); // Mostramos el texto "Temp: "
    lcd.print(temperature);
    lcd.print(" C");
  }
  
  void showHumidity(float humidity) override {
    lcd.setCursor(0, 1); // Nos movemos al inicio de la segunda fila
    lcd.print("Humedad: "); // Mostramos el texto "Humedad: "
    lcd.print(humidity);
    lcd.print(" %");
  }
};

#endif