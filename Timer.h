#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:
  Timer(unsigned long interval);
  bool expired();
  void start();
  void stop();
  void reset();
  bool isActive() const;

private:
  unsigned long interval;
  unsigned long previousMillis;
  bool active;
};

#endif // TIMER_H