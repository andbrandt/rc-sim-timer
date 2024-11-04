#ifndef LightDevice_h
#define LightDevice_h

#include <Arduino.h> 
#include <SevenSegmentAsciiMap.h>
#include <SevenSegmentExtended.h>
#include <SevenSegmentFun.h>
#include "SevenSegmentTM1637.h"

#include "HAL.hpp"

class LightDevice {
  public:
    virtual void Begin(int levelOff, int levelOn);
    virtual void Poll();      // time in ms.
    void Blink(bool enable, int periodOn_ms, int periodOff_ms);
    void On();
    void Off();

  protected:
    int m_levelOff;
    int m_levelOn;
    int m_level;
    void BlinkService();
    virtual void SetLevel(int level);

  private:
    bool m_blinkEnable=false;
    unsigned int m_blinkPeriodOn;
    unsigned int m_blinkPeriodOff;
    unsigned long m_blinkTriggerTimeOn;
    unsigned long m_blinkTriggerTimeOff;
    bool m_blinkOnState;
};
#endif