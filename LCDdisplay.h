#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include <LiquidCrystal.h>
#include "Display.h"

class LCDDisplay : public Display {
private:
  LiquidCrystal lcd;
public:
  LCDDisplay(uint8_t rs, uint8_t enable, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
    : lcd(rs, enable, d4, d5, d6, d7) {}
  
  void begin() override {
    lcd.begin(16, 2);
  }
  
  void showTemperature(float temperature) override {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
  }
  
  void showHumidity(float humidity) override {
    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print(" %");
  }
};

#endif