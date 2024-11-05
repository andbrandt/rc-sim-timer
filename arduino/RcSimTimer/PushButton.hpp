#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <Arduino.h> 
#include "UiEvent.hpp" 

class PushButton {
  public: // Overloaded base class functions
    enum ButtonEvent {
      LONG_PRESS,
      SHORT_PRESS,
    };
    virtual void PushButton::Begin(int pinPushButton, UiEvent *uiEvent, UiEvent::UiEventsExternal longPressEvent, UiEvent::UiEventsExternal shortPressX1Event, UiEvent::UiEventsExternal shortPressX2Event);
    virtual void Poll();      // time in ms.
    void ButtonScanService();

  protected:
    int m_pinPushButton;

  private:
    UiEvent         *m_uiEvent;
    unsigned long   m_buttonDownTimestamp = 0;
    bool            m_longPressServed = false;
    const int       m_buttonPressLongDuration = 1500;  // Must be pressed for at least 2000ms
    const int       m_buttonPressShortDuration = 50;  // Must be pressed for at least 50ms
    unsigned int    m_shortPressCounter = 1;
    unsigned long   m_shortPressTimeKeeper = 0;

    UiEvent::UiEventsExternal  m_longPressEvent;
    UiEvent::UiEventsExternal  m_shortPressX1Event;
    UiEvent::UiEventsExternal  m_shortPressX2Event;

    void HandleButtonEvent(ButtonEvent event);
};
#endif