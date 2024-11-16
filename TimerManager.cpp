#include "TimerManager.h"

TimerManager::TimerManager() : timerCount(0) {}

void TimerManager::addTimer(const String& name, unsigned long interval) {
  if (timerCount < 10) {
    timers[timerCount++] = TimerEntry(name, interval);
  }
}

bool TimerManager::shouldRun(const String& name) {
  TimerEntry* entry = findTimer(name);
  if (entry) {
    return entry->timer.shouldRun();
  }
  return false;
}

void TimerManager::start(const String& name) {
  TimerEntry* entry = findTimer(name);
  if (entry) {
    entry->timer.start();
  }
}

void TimerManager::stop(const String& name) {
  TimerEntry* entry = findTimer(name);
  if (entry) {
    entry->timer.stop();
  }
}

void TimerManager::reset(const String& name) {
  TimerEntry* entry = findTimer(name);
  if (entry) {
    entry->timer.reset();
  }
}

bool TimerManager::isActive(const String& name) const {
  TimerEntry* entry = findTimer(name);
  if (entry) {
    return entry->timer.isActive();
  }
  return false;
}

TimerManager::TimerEntry* TimerManager::findTimer(const String& name) {
  for (int i = 0; i < timerCount; ++i) {
    if (timers[i].name == name) {
      return &timers[i];
    }
  }
  return nullptr;
}