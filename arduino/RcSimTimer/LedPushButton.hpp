#ifndef LED_PUSHBUTTON_h
#define LED_PUSHBUTTON_h

#include <Arduino.h> 
#include "LED.hpp"
#include "PushButton.hpp"

class LedPushButton:public LED, public PushButton {
  public: // Overloaded base class functions
    void Begin(int pinLed, int levelOff, int levelOn, int pinPushButton, UiEvent *uiEvent, UiEvent::UiEventsExternal longPressEvent, UiEvent::UiEventsExternal shortPressX1Event, UiEvent::UiEventsExternal shortPressX2Event);
    void Poll() override;
};
#endif