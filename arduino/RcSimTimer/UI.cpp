#include "Arduino.h"

#include "UI.hpp"
#include "Time.hpp"
// Time time;

UI::UI() {
  #define QUEUE_SIZE_ITEMS 10
  m_eventQueue = new ArduinoQueue<UI::UiEvent>(10);
}

void UI::Begin(Display7Seg *display7Seg, LedPushButton *ledPushButton) {
  m_display7Seg   = display7Seg;
  m_ledPushButton = ledPushButton;

  m_aircraftSelection     = Easy;
  m_simDurationSelection  = SimDuration_2_00;

  m_simulator = new SimulatorPhoenix;
  EventPush(Reset);
}

void UI::Poll() {
  EventService();
  CountDownService();
}

void UI::EventPush(UiEvent event) {
  if (!m_eventQueue->isFull()) m_eventQueue->enqueue(event);
}

void UI::StateSet(UiState state) {
  m_state = state;
    
  switch(m_state) {
    case ModelSelect:
      StopCountDown();
      m_display7Seg->Print("    ");
      m_display7Seg->SetColonOn(false);
      m_display7Seg->Blink(true, 800, 200);
      m_ledPushButton->Off();
      break;

    case TimeSelect:
      m_display7Seg->SetColonOn(true);
      m_display7Seg->Blink(true, 800, 200);
      m_ledPushButton->Off();
      break;

    case SimArmed:
      StopCountDown();
      m_display7Seg->Off();
      m_ledPushButton->On();
      break;

    case SimRunning:
      m_display7Seg->Print("    ");
      m_display7Seg->SetColonOn(true);
      m_display7Seg->On();
      m_ledPushButton->On();
      break;

    case SimEnding:
      m_display7Seg->Blink(true, 500, 500);
      m_ledPushButton->Blink(true, 500, 500);
      break;

    case SimReArming:
      m_display7Seg->Off();
      m_ledPushButton->Off();
      break;

    default:
      // DEFAULT BRANCH SHOULD NEVER HAPPEN !!!
      break;
  }
}

void UI::SelectSimAircraft() {
  switch(m_aircraftSelection) {
    case Easy:
      m_simulator->EasyAircraft();
      break;
    default:
      m_simulator->AcroAircraft();
      break;
  }
}

void UI::EventService() {
  if (m_eventQueue->isEmpty()) return;

  auto event = m_eventQueue->dequeue();
  switch (event) {
    case Reset:
      StopCountDown();
      StateSet(SimArmed);
      break;

    case Setup:
      switch(m_state) {
        case ModelSelect:
          StateSet(TimeSelect);
          m_display7Seg->Print(m_simDurationSelectionString[m_simDurationSelection]);
          break;

        case TimeSelect:
          StateSet(SimArmed);
          break;

        default:
          StateSet(ModelSelect);
          SelectSimAircraft();
          m_display7Seg->Print(m_aircraftSelectionString[m_aircraftSelection]);
          break;
      }
      break;

    case Enter:
      switch(m_state) {
        case ModelSelect:
          m_aircraftSelection = m_aircraftSelection+1;
          if (m_aircraftSelection == last_aircraft) m_aircraftSelection = 0;
          SelectSimAircraft();
          m_display7Seg->Print(m_aircraftSelectionString[m_aircraftSelection]);
          break;

        case TimeSelect:
          m_simDurationSelection = m_simDurationSelection+1;
          if (m_simDurationSelection == last_simduration) m_simDurationSelection = 0;
          m_display7Seg->Print(m_simDurationSelectionString[m_simDurationSelection]);
          break;

        case SimArmed:
          StartCountDown(m_simDuration_ms[m_simDurationSelection]);
          StateSet(SimRunning);
          m_simulator->Reset();
          m_simulator->UnBlock();
          break;
        
        case SimRunning:
          m_simulator->Reset();
          break;

        case SimEnding:
          m_simulator->Reset();
          break;

        // cases SimEnding &  SimReArming are updated by countdown events
        default:
            break;
      }
      break;

    case TimeNearEnd:
      StateSet(SimEnding);
      break;

    case TimeAtEnd:
      StateSet(SimReArming);
      m_simulator->Block();
      break;

    case TimeArmed:
      StateSet(SimArmed);
      break;

    case TimeUpdate:
      // int timeRemaining = eventTime.timeRemaining / 1000;
      int timeRemaining = (m_timeEndTrigger_ms - time.Now_ms())/1000;
      unsigned int simTimeRemainingMin = timeRemaining / 60;
      unsigned int simTimeRemainingSec = timeRemaining % 60;

      char buf[5];
      if (simTimeRemainingMin>0) {
      sprintf(buf, " %1d%02d", simTimeRemainingMin, simTimeRemainingSec);
      } else {
        sprintf(buf, "  %02d", simTimeRemainingSec);
      }
      m_display7Seg->Print(buf);
      break;

    default:
      break;
  }
}

// ------------------- CountDown related functions ----------------------

void
UI::StartCountDown(unsigned long countDownPeriod_ms) {
  // UiEventTime eventTime;
UiEvent       event;

  m_timeEndTrigger_ms     = time.Now_ms() + countDownPeriod_ms;                   // event at time end  
  m_timeNearEndTrigger_ms = m_timeEndTrigger_ms - m_timeNearEndPeriod_ms;   // event x msec before time end to warn user
  m_timeArmed_ms   = m_timeEndTrigger_ms + m_timeRearmingPeriod_ms;         // event x msec after time end to nudge user to leave
  m_timeUpdateTrigger_ms  = time.Now_ms();
  m_countingDown = true;
}

void
UI::StopCountDown() {
  m_countingDown = false;
}

unsigned long
UI::GetTimeRemaining(unsigned long time_ms) {
  return (m_timeEndTrigger_ms == -1) ? 0 : m_timeEndTrigger_ms - time_ms;
}

void UI::CountDownService() {
  if (m_countingDown) {
    if (m_timeUpdateTrigger_ms < time.Now_ms()) {
      EventPush(TimeUpdate);
      m_timeUpdateTrigger_ms += 1000;
    }

    if (m_timeNearEndTrigger_ms < time.Now_ms()) {
      EventPush(TimeNearEnd);
      m_timeNearEndTrigger_ms = -1;
    }

    if (m_timeEndTrigger_ms < time.Now_ms() - time.GetTimerInterval_ms()) {
      EventPush(TimeAtEnd);
      m_timeEndTrigger_ms = -1;
      m_timeUpdateTrigger_ms = -1;
    }

    if (m_timeArmed_ms < time.Now_ms()) {
      EventPush(TimeArmed);
      m_timeArmed_ms = -1;
    }
  }
}