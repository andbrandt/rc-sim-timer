#ifndef Display7Seg_h
#define Display7Seg_h

#include <Arduino.h> 
#include "LightDevice.hpp"

#include <SevenSegmentAsciiMap.h>
#include <SevenSegmentExtended.h>
#include <SevenSegmentFun.h>
#include "SevenSegmentTM1637.h"

class Display7Seg:public LightDevice, public SevenSegmentTM1637
{

  public: // Overloaded base class functions
    Display7Seg(int pinClk, int pinSda, int levelOff, int levelOn);
    void Begin();
    void SetLevel(int level) override;       // Overloaded for 7 segment display backlight level control
};
#endif