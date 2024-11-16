#include "Timer.h"

Timer::Timer(unsigned long interval) : interval(interval), previousMillis(0), active(false) {}

bool Timer::shouldRun() {
  if (!active) return false;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}

void Timer::start() {
  active = true;
  previousMillis = millis();
}

void Timer::stop() {
  active = false;
}

void Timer::reset() {
  previousMillis = millis();
}

bool Timer::isActive() const {
  return active;
}