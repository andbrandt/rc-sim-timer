#include "Arduino.h"

#include "Display7Seg.hpp"
#include "HAL.hpp"


Display7Seg::Display7Seg(int pinClk, int pinSda, int levelOff, int levelOn) : LightDevice(levelOff, levelOn) 
{
  m_pinClk      = pinClk;
  m_pinSda      = pinSda;
  m_colonEnable = false;

  m_display = new SevenSegmentTM1637(m_pinClk, m_pinSda);
  m_display->begin();
}

void Display7Seg::Print(char *buf) {
  if (m_display) m_display->print(buf);
}

void Display7Seg::SetColonOn(bool enable) {
  if (m_display) m_display->setColonOn(enable);
}

void Display7Seg::SetLevel(int level) {
  LightDevice::SetLevel(level);
  // DEBUG_PRINT("Display7Seg::SetLevel");
  m_display->setBacklight(m_level);
}