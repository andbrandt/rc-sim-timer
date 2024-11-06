#include "Arduino.h"

#include "UI.hpp"
#include "Time.hpp"

#include "Simulator.hpp"
#include "SimulatorPhoenix.hpp"
#include "SimulatorReflex.hpp"

UI::UI() {
  #define QUEUE_SIZE_ITEMS 10
  m_eventQueue = new ArduinoQueue<UI::UiEventsInternal>(10);
}

void UI::Begin(Display7Seg *display7Seg, LedPushButton *ledPushButton) {
  m_display7Seg   = display7Seg;
  m_ledPushButton = ledPushButton;

  m_aircraftSelection     = 0;
  m_simDurationSelection  = 0;
  m_simAppSelection       = 0;
  m_simAppSelection_apps[SimApp_Phoenix] = new SimulatorPhoenix;
  m_simAppSelection_apps[SimApp_Reflex] = new SimulatorReflex;
  m_simulator = m_simAppSelection_apps[m_simAppSelection];

  m_display7Seg->SetColonOn(true);
  m_display7Seg->Print(m_versionString);
  delay(3000);
  EventPush(Reset);
}

void UI::Poll() {
  EventService();
  CountDownService();
}

void UI::StateSet(UiState state) {
  m_state = state;
    
  switch(m_state) {
    case SimulatorSelect:
      StopCountDown();
      m_display7Seg->SetColonOn(false);
      m_display7Seg->Blink(true, 800, 200);
      m_ledPushButton->Off();
      m_display7Seg->Print(m_simAppSelectionString[m_simAppSelection]);
      break;

    case TimeSelect:
      m_display7Seg->SetColonOn(true);
      m_display7Seg->Blink(true, 800, 200);
      m_ledPushButton->Off();
      m_display7Seg->Print(m_simDurationSelectionString[m_simDurationSelection]);
      break;

    case SimArmed:
      StopCountDown();
      m_display7Seg->SetColonOn(false);
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
      m_simulator->UnBlock();
      m_simulator->Block();
      StateSet(SimArmed);
      break;

    case Setup:
      DEBUG_PRINT("Setup");
      DEBUG_PRINT(m_state);
      switch(m_state) {
        case TimeSelect:
          StateSet(SimArmed);
          break;

        case SimulatorSelect:
          StateSet(TimeSelect);
          break;

        default:
          StateSet(SimulatorSelect);
          m_simulator->UnBlock();
          m_simulator->Block();
          break;
      }
      break;

    case Enter:
      switch(m_state) {
        case SimulatorSelect:
          m_simAppSelection = m_simAppSelection+1;
          if (m_simAppSelection == last_simApp) m_simAppSelection = 0;
          m_simulator = m_simAppSelection_apps[m_simAppSelection];
          StateSet(m_state);
          break;

        case TimeSelect:
          m_simDurationSelection = m_simDurationSelection+1;
          if (m_simDurationSelection == last_simDuration) m_simDurationSelection = 0;
          StateSet(m_state);
          break;

        case SimArmed:
          StartCountDown(m_simDuration_ms[m_simDurationSelection]);
          StateSet(SimRunning);
          m_simulator->UnBlock();
          m_simulator->Reset();
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

    case ModelToggle:
      if (m_state == SimArmed || m_state == SimRunning || m_state == SimEnding) {
        m_aircraftSelection = m_aircraftSelection+1;
        if (m_aircraftSelection == last_aircraft) m_aircraftSelection = 0;
        SelectSimAircraft();
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