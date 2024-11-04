#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <Arduino.h> 

class PushButton {
  public: // Overloaded base class functions
    enum ButtonEvent {
      LONG_PRESS,
      SHORT_PRESS,
    };
    virtual void Begin(int pinPushButton, void *CallbackEvent);
    virtual void Poll();      // time in ms.
    void ButtonScanService();

  protected:
    int m_pinPushButton;

  private:
    void (*m_CallbackEvent)(ButtonEvent event);
    bool m_longPressServed = false;
    bool m_ButtonDown = false;
    const int m_buttonPressLongDuration = 2000;  // Must be pressed for at least 2000ms
    const int m_buttonPressShortDuration = 50;  // Must be pressed for at least 50ms
    unsigned long m_buttonDownTimestamp = 0;
};
#endif