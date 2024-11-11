#include "Arduino.h"

#include "Display7Seg.hpp"
#include "HAL.hpp"


Display7Seg::Display7Seg(int pinClk, int pinSda, int levelOff, int levelOn) : SevenSegmentTM1637(pinClk, pinSda), LightDevice(levelOff, levelOn)
{
}

void Display7Seg::Begin()
{
  SevenSegmentTM1637::begin();
}

void Display7Seg::SetLevel(int level) 
{
  LightDevice::SetLevel(level);
  // DEBUG_PRINT("Display7Seg::SetLevel");
  SevenSegmentTM1637::setBacklight(level);
}