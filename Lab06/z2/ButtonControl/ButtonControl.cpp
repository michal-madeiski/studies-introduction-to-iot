#include "ButtonControl.h"

void ButtonControl::init(int pin, unsigned long debounceTime, unsigned long longPressTime)
{
  pin_ = pin;
  debounceTime_ = debounceTime;
  longPressTime_ = longPressTime;
  lastState_ = HIGH;
  isPressed_ = false;
  lastChangeTime_ = 0;
  pressStartTime_ = 0;
  pinMode(pin_, INPUT_PULLUP);
  shortPressCallback_ = nullptr;
  longPressCallback_ = nullptr;
}

void ButtonControl::setShortPressCallback(void (*callback)())
{
  shortPressCallback_ = callback;
}

void ButtonControl::setLongPressCallback(void (*callback)())
{
  longPressCallback_ = callback;
}

void ButtonControl::runMeInLoop()
{
  bool currentState = digitalRead(pin_);
  unsigned long now = millis();

  if (currentState != lastState_) {
    lastChangeTime_ = now;
    lastState_ = currentState;
  }

  if (!isPressed_ && currentState == LOW && (now - lastChangeTime_) > debounceTime_) {
    isPressed_ = true;
    pressStartTime_ = now;
  }

  if (isPressed_ && currentState == HIGH && (now - lastChangeTime_) > debounceTime_) {
    unsigned long pressDuration = now - pressStartTime_;
    if (pressDuration >= longPressTime_) {
      if (longPressCallback_) longPressCallback_();
    } else {
      if (shortPressCallback_) shortPressCallback_();
    }
    isPressed_ = false;
  }
}