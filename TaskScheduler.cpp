#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(unsigned long interval) : interval(interval), previousMillis(0) {}

bool TaskScheduler::shouldRun() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}