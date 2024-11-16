#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(uint8_t pin) : pin(pin), dryValue(0), wetValue(1023) {}

void SoilMoistureSensor::begin() {
  pinMode(pin, INPUT);
}

float SoilMoistureSensor::readValue() {
  int analogValue = analogRead(pin);
  // Convertir el valor analógico en porcentaje de humedad
  float humidity = map(analogValue, dryValue, wetValue, 0, 100);
  humidity = constrain(humidity, 0, 100); // Asegurarse de que el valor esté entre 0 y 100
  return humidity;
}

void SoilMoistureSensor::calibrate(float dryValue, float wetValue) {
  this->dryValue = dryValue;
  this->wetValue = wetValue;
}