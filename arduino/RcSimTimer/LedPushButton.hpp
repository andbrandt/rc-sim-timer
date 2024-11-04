#ifndef LED_PUSHBUTTON_h
#define LED_PUSHBUTTON_h

#include <Arduino.h> 
#include "LED.hpp"
#include "PushButton.hpp"

class LedPushButton:public LED, public PushButton {
  public: // Overloaded base class functions
    void Begin(int pinLed, int levelOff, int levelOn, int pinPushButton, void *CallbackEvent);
    void Poll();
};
#endif