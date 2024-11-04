#include "Arduino.h"

#include "PushButton.hpp"
#include "HAL.hpp"
#include "Time.hpp"
// Time time;

void PushButton::Begin(int pinPushButton, void *callbackEvent) {
  DEBUG_PRINT("PushButton::Begin");
  m_pinPushButton = pinPushButton;
  m_CallbackEvent = callbackEvent;

  pinMode(m_pinPushButton, INPUT_PULLUP);
}

void PushButton::Poll() {
  ButtonScanService();
}

void PushButton::ButtonScanService() {
  if (digitalRead(m_pinPushButton)) {
    // Button not pressed
    if (m_buttonDownTimestamp) {
      // Timestamp is not zero => button press just ended
      if ((time.Now_ms() - m_buttonDownTimestamp) > m_buttonPressLongDuration) {
        // Ignore end of long press trigger - this was already handled when the IntervalLong interval was reached
        m_buttonDownTimestamp = 0;
        m_longPressServed = false;
      } else if ((time.Now_ms() - m_buttonDownTimestamp) > m_buttonPressShortDuration) {
        // Just released after a short press
        m_buttonDownTimestamp = 0;
        m_CallbackEvent(SHORT_PRESS);
      }
    }
  } else {
    // Button IS pressed
    if (!m_buttonDownTimestamp) {
      // Timestamp is zero => button press just started
      m_buttonDownTimestamp = time.Now_ms();
    }
    // Button IS pressed
    if ((time.Now_ms() - m_buttonDownTimestamp) > m_buttonPressLongDuration) {
      // Button was pressed for a long time
      if (!m_longPressServed) {
        m_CallbackEvent(LONG_PRESS);
        m_longPressServed = true;
      }
    }
  }

}