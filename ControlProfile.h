#ifndef CONTROLPROFILE_H
#define CONTROLPROFILE_H

class ControlProfile {
public:
  ControlProfile(float tempThreshold, float humThreshold, float tempHysteresis, float humHysteresis,
                 float minTemp, float maxTemp, float minHum, float maxHum,
                 float minSoilMoisture, float maxSoilMoisture, float soilMoistureThreshold, unsigned long infiltrationTime, unsigned long irrigationTime);
  
  float getTempThreshold() const;
  float getHumThreshold() const;
  float getTempHysteresis() const;
  float getHumHysteresis() const;
  float getMinTemp() const;
  float getMaxTemp() const;
  float getMinHum() const;
  float getMaxHum() const;
  float getMinSoilMoisture() const;
  float getMaxSoilMoisture() const;
  float getSoilMoistureThreshold() const;
  unsigned long getInfiltrationTime() const;
  unsigned long getIrrigationTime() const;

private:
  float tempThreshold;
  float humThreshold;
  float tempHysteresis;
  float humHysteresis;
  float minTemp;
  float maxTemp;
  float minHum;
  float maxHum;
  float minSoilMoisture;
  float maxSoilMoisture;
  float soilMoistureThreshold;
  unsigned long infiltrationTime;
  unsigned long irrigationTime;
};

#endif // CONTROLPROFILE_H