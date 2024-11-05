#include "Arduino.h"

#include "LedPushButton.hpp"
#include "HAL.hpp"

void LedPushButton::Begin(int pinLed, int levelOff, int levelOn, int pinPushButton, UiEvent *uiEvent, UiEvent::UiEventsExternal longPressEvent, UiEvent::UiEventsExternal shortPressX1Event, UiEvent::UiEventsExternal shortPressX2Event) {
  LED::Begin(pinLed, levelOff, levelOn);
  PushButton::Begin(pinPushButton, uiEvent, longPressEvent, shortPressX1Event, shortPressX2Event);

  m_pinLed        = pinLed;
  m_pinPushButton = pinPushButton;
  DEBUG_PRINT("LedPushButton::Begin");
}

void LedPushButton::Poll() {      // time in ms.
  LED::Poll();
  PushButton::Poll();
}
