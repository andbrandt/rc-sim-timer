#include "Arduino.h"

#include "UI.hpp"
#include "UiEvent.hpp"

void UiEvent::EventPush(UiEventsExternal event) 
{
    DEBUG_PRINT("UiEvent::EventPush");
    DEBUG_PRINT(event);
  if (!m_eventQueue->isFull()) m_eventQueue->enqueue((UiEventsInternal)event);
}

void UiEvent::EventPush(UiEventsInternal event) 
{
    DEBUG_PRINT("UiEvent::EventPush");
    DEBUG_PRINT(event);
  if (!m_eventQueue->isFull()) m_eventQueue->enqueue(event);
}