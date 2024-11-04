#ifndef LED_h
#define LED_h

#include <Arduino.h> 
#include "LightDevice.hpp"

class LED:public LightDevice {
  public: // Overloaded base class functions
    virtual void Begin(int pinLed, int levelOff, int levelOn);
    virtual void Poll();      // time in ms. Not needed in LED but LED is parent to LedPushButton
    void SetLevel(int level);

  protected:
    int m_pinLed;
};
#endif