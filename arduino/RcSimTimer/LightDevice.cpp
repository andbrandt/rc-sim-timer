#include "Arduino.h"

#include "LightDevice.hpp"
#include "HAL.hpp"
#include "Time.hpp"
// Time time;

LightDevice::LightDevice(int levelOff, int levelOn)
{
  m_levelOff    = levelOff;
  m_levelOn     = levelOn;
}

void LightDevice::Poll() {      // time in ms.
  BlinkService();
}

void LightDevice::Blink(bool enable, int periodOn, int periodOff) {     // Blink per second; Start at next poll event; Start with ON state
  // DEBUG_PRINT("LightDevice::Blink");
  m_blinkEnable = enable;
  if (m_blinkEnable) {
    m_blinkPeriodOn = periodOn;
    m_blinkPeriodOff = periodOff;
    m_blinkTriggerTimeOn = time.Now_ms();
    m_blinkTriggerTimeOff = m_blinkTriggerTimeOn + m_blinkPeriodOn;
    SetLevel(m_levelOn);
  } else {
    m_blinkTriggerTimeOn = 0;
    m_blinkTriggerTimeOff = 0;
    SetLevel(m_levelOff);
  }
}

// Service is inactive if m_blinkPeriodOn is zero
void LightDevice::BlinkService() {
  if (m_blinkEnable) {
    if (m_blinkTriggerTimeOn < time.Now_ms()) {
      SetLevel(m_levelOn);
      m_blinkTriggerTimeOn += m_blinkPeriodOn + m_blinkPeriodOff;
    }

    if (m_blinkTriggerTimeOff < time.Now_ms()) {
      SetLevel(m_levelOff);
      m_blinkTriggerTimeOff += m_blinkPeriodOn + m_blinkPeriodOff;
    }
  }
}

void LightDevice::On() {
  m_blinkEnable=false;
  SetLevel(m_levelOn);
}

void LightDevice::Off() {
  m_blinkEnable=false;
  SetLevel(m_levelOff);
}

void LightDevice::SetLevel(int level) {
  // DEBUG_PRINT("LightDevice::SetLevel");
  m_level=level;
  // Leave it for specialization functions to do actual HW writing
}