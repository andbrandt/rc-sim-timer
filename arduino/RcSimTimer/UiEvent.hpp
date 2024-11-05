#ifndef UIEVENT_H
#define UIEVENT_H

#include <Arduino.h> 
#include <ArduinoQueue.h>

class UiEvent {
  public:
    enum UiEventsExternal {
      Reset,
      Setup,
      Enter,
      UiEventsExternalLast,
    };  

    enum UiEventsInternal {
      TimeNearEnd=UiEvent::UiEventsExternalLast,
      TimeAtEnd,
      TimeArmed,
      TimeUpdate,
    };  

    void EventPush(UiEventsExternal event);
    void EventPush(UiEventsInternal event);

  protected:
    ArduinoQueue<UiEvent::UiEventsInternal> *m_eventQueue;
};
#endif