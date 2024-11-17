#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <Arduino.h>

class TaskScheduler {
public:
  TaskScheduler(unsigned long interval);
  bool shouldRun();

private:
  unsigned long interval;
  unsigned long previousMillis;
};

#endif // TASKSCHEDULER_H