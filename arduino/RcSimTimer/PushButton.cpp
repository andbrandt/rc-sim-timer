#include "Arduino.h"

#include "PushButton.hpp"
#include "HAL.hpp"
#include "Time.hpp"
// Time time;

PushButton::PushButton(int pinPushButton) {
  m_pinPushButton = pinPushButton;
  pinMode(m_pinPushButton, INPUT_PULLUP);
}

void PushButton::Begin(UiEvent *uiEvent, UiEvent::UiEventsExternal longPressEvent, UiEvent::UiEventsExternal shortPressX1Event, UiEvent::UiEventsExternal shortPressX2Event) {
  DEBUG_PRINT("PushButton::Begin");
  DEBUG_PRINT(longPressEvent);
  DEBUG_PRINT(shortPressX1Event);
  DEBUG_PRINT(shortPressX2Event);

  m_uiEvent = uiEvent;

  m_longPressEvent    = longPressEvent;
  m_shortPressX1Event = shortPressX1Event;
  m_shortPressX2Event = shortPressX2Event;
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
        HandleButtonEvent(SHORT_PRESS);
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
        HandleButtonEvent(LONG_PRESS);
        m_longPressServed = true;
      }
    }
  }
}

void PushButton::HandleButtonEvent(ButtonEvent event) {
  DEBUG_PRINT("PushButton::HandleButtonEvent");
  DEBUG_PRINT(event);
  switch (event) {
    case PushButton::LONG_PRESS:
      m_uiEvent->EventPush(m_longPressEvent);
      break;
    case PushButton::SHORT_PRESS:
      if ((time.Now_ms() - m_shortPressTimeKeeper) < m_buttonPressLongDuration/4) {
        m_shortPressCounter++;
      } else {
        m_shortPressCounter = 1;
      }
      m_shortPressTimeKeeper = time.Now_ms();
      m_uiEvent->EventPush((m_shortPressCounter==1) ? m_shortPressX1Event : m_shortPressX2Event);

      break;
    default:
        break;
  }
}