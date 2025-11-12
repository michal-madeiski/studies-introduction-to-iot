#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include "Arduino.h"

typedef unsigned long Time;

class ButtonControl
{
public:
  void init(int pin, unsigned long debounceTime, unsigned long longPressTime);
  void setShortPressCallback(void (*callback)());
  void setLongPressCallback(void (*callback)());
  void runMeInLoop();

private:
  int pin_;
  Time debounceTime_;
  Time longPressTime_;
  Time lastChangeTime_;
  Time pressStartTime_;
  bool lastState_;
  bool isPressed_;
  void (*shortPressCallback_)();
  void (*longPressCallback_)();
};

#endif