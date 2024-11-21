#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H

#include <Arduino.h>
#include "Timer.h"

class TimerManager {
public:
  TimerManager();
  void addTimer(const String& name, unsigned long interval);
  bool expired(const String& name);
  void start(const String& name);
  void stop(const String& name);
  void reset(const String& name);
  bool isActive(const String& name) const;

private:
  struct TimerEntry {
    String name;
    Timer timer;
    TimerEntry() : name(""), timer(0) {} // Constructor por defecto
    TimerEntry(const String& name, unsigned long interval) : name(name), timer(interval) {}
  };

  TimerEntry* findTimer(const String& name);

  TimerEntry timers[10]; // Ajustar el tamaño según  necesidades
  int timerCount;
};

#endif // TIMERMANAGER_H