#include "LCDDisplay.h"

// Constructor de la clase LCDDisplay
LCDDisplay::LCDDisplay(int rs, int rw, int enable, int d4, int d5, int d6, int d7)
  : lcd(rs, enable, d4, d5, d6, d7), rwPin(rw) {}

// Implementación del método begin
void LCDDisplay::begin() {
  // El pin RW lo dejamos en bajo ya que siempre estaremos escribiendo en la pantalla.
  pinMode(rwPin, OUTPUT);
  digitalWrite(rwPin, LOW);

  lcd.begin(16, 2); 
  lcd.clear();
}

// Implementación del método showTemperature
void LCDDisplay::showTemperature(float temperature) {
  lcd.setCursor(0, 0); // Establece el cursor en la primera columna de la primera fila
  lcd.print("Temp: ");
  lcd.print(temperature, 2); 
  lcd.print(" C");
}

// Implementación del método showHumidity
void LCDDisplay::showHumidity(float humidity) {
  lcd.setCursor(0, 1); // Establece el cursor en la primera columna de la segunda fila
  lcd.print("Hum: ");
  lcd.print(humidity, 2);
  lcd.print(" %");
}

// Implementación del método showMessage
void LCDDisplay::showMessage(int x, int y, const char* message) {
  lcd.setCursor(x, y); // Establece el cursor en la columna x y fila y indicadas
  lcd.print(message);
}

// Implementación del método clear
void LCDDisplay::clear() {
  lcd.clear();
}