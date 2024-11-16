#ifndef DISPLAY_H
#define DISPLAY_H

class Display {
public:
  virtual void begin() = 0;
  virtual void showTemperature(float temperature) = 0;
  virtual void showHumidity(float humidity) = 0;
  virtual void showMessage(int x, int y, const char* message) = 0;
  virtual void clear() = 0;
};

#endif