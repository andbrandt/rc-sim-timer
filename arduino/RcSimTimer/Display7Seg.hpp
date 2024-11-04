#ifndef Display7Seg_h
#define Display7Seg_h

#include <Arduino.h> 
#include "LightDevice.hpp"

#include <SevenSegmentAsciiMap.h>
#include <SevenSegmentExtended.h>
#include <SevenSegmentFun.h>
#include "SevenSegmentTM1637.h"

class Display7Seg:public LightDevice
{

  public: // Overloaded base class functions
    void Begin(int pinClk, int pinSda, int levelOff, int levelOn);
    void SetLevel(int level) override;       // Overloaded for 7 segment display control rather than PCB LED

  public: // Additional derived class functions
    void Print(char *buf);
    void SetColonOn(bool enable);

  private:   
    int m_pinClk;
    int m_pinSda;
    bool m_colonEnable=false;
    SevenSegmentTM1637  *m_display;
};
#endif