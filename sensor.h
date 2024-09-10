#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
public:
  virtual void begin() = 0;
  virtual float readTemperature() = 0;
  virtual float readHumidity() = 0;
};

#endif