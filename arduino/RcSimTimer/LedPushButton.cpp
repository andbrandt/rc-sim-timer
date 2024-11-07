#include "Arduino.h"

#include "LedPushButton.hpp"
#include "HAL.hpp"

LedPushButton::LedPushButton(int pinLed, int levelOff, int levelOn, int pinPushButton) : LED(pinLed, levelOff, levelOn), PushButton(pinPushButton)
{
}

void LedPushButton::Begin(UiEvent *uiEvent, UiEvent::UiEventsExternal longPressEvent, UiEvent::UiEventsExternal shortPressX1Event, UiEvent::UiEventsExternal shortPressX2Event) 
{
  PushButton::Begin(uiEvent, longPressEvent, shortPressX1Event, shortPressX2Event);
  DEBUG_PRINT("LedPushButton::Begin");
}

void LedPushButton::Poll() 
{
  LED::Poll();
  PushButton::Poll();
}
