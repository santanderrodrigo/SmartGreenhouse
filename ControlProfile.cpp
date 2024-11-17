#include "ControlProfile.h"

ControlProfile::ControlProfile(float tempThreshold, float humThreshold, float tempHysteresis, float humHysteresis,
                               float minTemp, float maxTemp, float minHum, float maxHum,
                               float minSoilMoisture, float maxSoilMoisture, float soilMoistureThreshold, unsigned long infiltrationTime, unsigned long irrigationTime)
  : tempThreshold(tempThreshold), humThreshold(humThreshold), tempHysteresis(tempHysteresis), humHysteresis(humHysteresis),
    minTemp(minTemp), maxTemp(maxTemp), minHum(minHum), maxHum(maxHum),
    minSoilMoisture(minSoilMoisture), maxSoilMoisture(maxSoilMoisture), soilMoistureThreshold(soilMoistureThreshold),
    infiltrationTime(infiltrationTime), irrigationTime(irrigationTime) {}

float ControlProfile::getTempThreshold() const {
  return tempThreshold;
}

float ControlProfile::getHumThreshold() const {
  return humThreshold;
}

float ControlProfile::getTempHysteresis() const {
  return tempHysteresis;
}

float ControlProfile::getHumHysteresis() const {
  return humHysteresis;
}

float ControlProfile::getMinTemp() const {
  return minTemp;
}

float ControlProfile::getMaxTemp() const {
  return maxTemp;
}

float ControlProfile::getMinHum() const {
  return minHum;
}

float ControlProfile::getMaxHum() const {
  return maxHum;
}

float ControlProfile::getMinSoilMoisture() const {
  return minSoilMoisture;
}

float ControlProfile::getMaxSoilMoisture() const {
  return maxSoilMoisture;
}

float ControlProfile::getSoilMoistureThreshold() const {
  return soilMoistureThreshold;
}

unsigned long ControlProfile::getInfiltrationTime() const {
  return infiltrationTime;
}

unsigned long ControlProfile::getIrrigationTime() const {
  return irrigationTime;
}