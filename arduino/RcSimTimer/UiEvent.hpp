#ifndef UIEVENT_H
#define UIEVENT_H

#include <Arduino.h> 
#include <ArduinoQueue.h>

class UiEvent {
  public:
    enum UiEventsExternal {
      Setup,
      Enter,
      ModelToggle,
      UiEventsExternalLast,
    };  

    enum UiEventsInternal {
      Reset=UiEventsExternalLast,
      TimeNearEnd,
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