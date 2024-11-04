#include "Arduino.h"

#include "LedPushButton.hpp"
#include "HAL.hpp"

void LedPushButton::Begin(int pinLed, int levelOff, int levelOn, int pinPushButton, void *CallbackEvent) {
  LED::Begin(pinLed, levelOff, levelOn);
  PushButton::Begin(pinPushButton, CallbackEvent);

  m_pinLed        = pinLed;
  m_pinPushButton = pinPushButton;
  DEBUG_PRINT("LedPushButton::Begin");
}

void LedPushButton::Poll() {      // time in ms.
  LED::Poll();
  PushButton::Poll();
}
