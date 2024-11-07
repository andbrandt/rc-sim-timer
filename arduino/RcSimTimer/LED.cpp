#include "Arduino.h"

#include "LED.hpp"
#include "HAL.hpp"

LED::LED(int pinLed, int levelOff, int levelOn) : LightDevice(levelOff, levelOn) 
{
  m_pinLed      = pinLed;
  pinMode(pinLed, OUTPUT);
}

void LED::Poll()       // time in ms.
{
  LightDevice::Poll();
}

void LED::SetLevel(int level) 
{
  LightDevice::SetLevel(level);
  digitalWrite(m_pinLed, m_level);
}