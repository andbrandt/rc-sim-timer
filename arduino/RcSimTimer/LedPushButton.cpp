#include "Arduino.h"

#include "LedPushButton.hpp"
#include "HAL.hpp"

void LedPushButton::Begin(int pinLed, int levelOff, int levelOn, int pinPushButton, void *CallbackEvent, int longPressEvent, int shortPressX1Event) {
  LED::Begin(pinLed, levelOff, levelOn);
  PushButton::Begin(pinPushButton, CallbackEvent, longPressEvent, shortPressX1Event);

  m_pinLed        = pinLed;
  m_pinPushButton = pinPushButton;
  DEBUG_PRINT("LedPushButton::Begin");
}

void LedPushButton::Poll() {      // time in ms.
  LED::Poll();
  PushButton::Poll();
}
