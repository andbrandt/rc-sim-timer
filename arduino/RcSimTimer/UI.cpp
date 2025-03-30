#include "Arduino.h"

#include "src/anduinolib/sys/Time.hpp"
#include "src/anduinolib/rc/sim/SimulatorBase.hpp"
#include "src/anduinolib/rc/sim/SimPhoenix.hpp"
#include "src/anduinolib/rc/sim/SimRealFlight9.hpp"
#include "src/anduinolib/rc/sim/SimRealFlightBasic.hpp"
#include "UI.hpp"

using namespace anduinolib::sys;

UI::UI() 
{
  #define QUEUE_SIZE_ITEMS 10
  m_eventQueue = new ArduinoQueue<UI::UiEventsInternal>(10);
}

// Return false if version or checksum does not match expected values
bool
UI::LoadSettings()
{
  DEBUG_PRINT("LoadSettings");

  // Pre-initialise version and size fields before trying to read from eeprom
  InitSettings();

  if (m_settings.version != EEPROM.read(0)) return false;
  if (m_settings.structSize != EEPROM.read(1)) return false;

  // Now we know that version and size looks OK
  DEBUG_PRINT("version and size looks OK");
  m_settings.simDurationSelection = EEPROM.read(2);
  m_settings.simAppSelection = EEPROM.read(3);

  // Calculate expected checksum
  CalcSettingsChkSum();
  if (m_settings.chkSum != EEPROM.read(4)) return false;
  DEBUG_PRINT("chkSum is OK");

  return true;
}

void
UI::SaveSettings()
{
  DEBUG_PRINT("SaveSettings");
  EEPROM.write(0, m_settings.version);
  EEPROM.write(1, m_settings.structSize);
  EEPROM.write(2, m_settings.simDurationSelection);
  EEPROM.write(3, m_settings.simAppSelection);
  CalcSettingsChkSum();
  EEPROM.write(4, m_settings.chkSum);
}

void
UI::CalcSettingsChkSum()
{
  m_settings.chkSum = m_settings.version +
                      m_settings.structSize +
                      m_settings.simDurationSelection +
                      m_settings.simAppSelection;
}

void
UI::InitSettings()
{
  m_settings.version              = 1;
  m_settings.structSize           = sizeof(m_settings);
  m_settings.simDurationSelection = 0;
  m_settings.simAppSelection      = 0;
  CalcSettingsChkSum();
}

void UI::Begin(Display7Seg *display7Seg, LedPushButton *ledPushButton, bool safemode)
{
  m_display7Seg   = display7Seg;
  m_ledPushButton = ledPushButton;

  if (!LoadSettings()) {
    InitSettings();
  }

  if (safemode) {
    DEBUG_PRINT("SafeMode");
    m_display7Seg->setColonOn(false);
    m_display7Seg->print("SAFE");
    delay(1000);
    m_display7Seg->print("mode");
    delay(1000);
    m_display7Seg->print("");
    m_display7Seg->setColonOn(true);
  }
  
  m_display7Seg->setColonOn(true);
  m_display7Seg->On();
  m_display7Seg->print(m_versionString);
  delay(3000);

  m_display7Seg->setColonOn(false);
  m_display7Seg->print(m_simAppSelectionString[m_settings.simAppSelection]);
  m_display7Seg->setColonOn(true);
  delay(3000);

  DEBUG_PRINT("Settings");
  DEBUG_PRINT(m_settings.simAppSelection);
  DEBUG_PRINT(m_settings.simDurationSelection);

  m_aircraftSelection     = 0;  // Some simulators use toggling, so this has to start at zero always
  m_simAppSelection_apps[SimApp_Phoenix4] = new SimulatorPhoenix;
  m_simAppSelection_apps[SimApp_RealFlight9] = new SimulatorRealFlight9;
  m_simAppSelection_apps[SimApp_RealFlightBasic] = new SimulatorRealFlightBasic;
  m_simulator = m_simAppSelection_apps[m_settings.simAppSelection];

  EventPush(Reset);
}

void UI::Poll() 
{
  EventService();
  CountDownService();
}

void UI::StateSet(UiState state) 
{
  m_state = state;
    
  switch(m_state) {
    case SimulatorSelect:
      StopCountDown();
      m_display7Seg->setColonOn(false);
      m_display7Seg->Blink(true, 800, 200);
      m_ledPushButton->Off();
      m_display7Seg->print(m_simAppSelectionString[m_settings.simAppSelection]);
      break;

    case TimeSelect:
      m_display7Seg->setColonOn(true);
      m_display7Seg->Blink(true, 800, 200);
      m_ledPushButton->Off();
      m_display7Seg->print(m_simDurationSelectionString[m_settings.simDurationSelection]);
      break;

    case SimArmed:
      StopCountDown();
      m_display7Seg->setColonOn(false);
      m_display7Seg->Off();
      m_ledPushButton->On();
      break;

    case SimRunning:
      m_display7Seg->print("    ");
      m_display7Seg->setColonOn(true);
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
      StateSet(SimArmed);
      StopCountDown();
      m_simulator->InitSim();
      delay(250);
      m_simulator->BlockSim();
      break;

    case Setup:
      DEBUG_PRINT("Setup");
      DEBUG_PRINT(m_state);
      switch(m_state) {
        case TimeSelect:
          SaveSettings();
          StateSet(SimArmed);
          break;

        case SimulatorSelect:
          StateSet(TimeSelect);
          break;

        default:
          StateSet(SimulatorSelect);
          m_simulator->BlockSim();
          break;
      }
      break;

    case Enter:
      switch(m_state) {
        case SimulatorSelect:

          if (++m_settings.simAppSelection == simApp_last) m_settings.simAppSelection = 0;
          m_simulator = m_simAppSelection_apps[m_settings.simAppSelection];
          StateSet(m_state);
          break;

        case TimeSelect:
          if (++m_settings.simDurationSelection == simDuration_last) m_settings.simDurationSelection = 0;
          StateSet(m_state);
          break;

        case SimArmed:
          StateSet(SimRunning);
          StartCountDown(m_simDuration_ms[m_settings.simDurationSelection]+1000);  // Also include time = 0 for best user experience
          m_simulator->RestartSim();
          break;
        
        case SimRunning:
          m_simulator->RestartSim();
          break;

        case SimEnding:
          m_simulator->RestartSim();
          break;

        // cases SimEnding &  SimReArming are updated by countdown events
        default:
            break;
      }
      break;

    case ModelToggle:
      if (m_state == SimArmed || m_state == SimRunning || m_state == SimEnding) {
        m_aircraftSelection = m_aircraftSelection+1;
        if (m_aircraftSelection == Aircraft_last) m_aircraftSelection = 0;
        SelectSimAircraft();
      }
    break;

    case TimeNearEnd:
      StateSet(SimEnding);
      break;

    case TimeAtEnd:
      StateSet(SimReArming);
      m_simulator->BlockSim();
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
      // if (timeRemaining>0) {
        if (simTimeRemainingMin>0) {
        sprintf(buf, " %1d%02d", simTimeRemainingMin, simTimeRemainingSec);
        } else {
          sprintf(buf, "  %02d", simTimeRemainingSec);
        }
        m_display7Seg->print(buf);
      // } else {
      //   m_display7Seg->print("    ");
      // }
      break;

    default:
      break;
  }
}

// ------------------- CountDown related functions ----------------------

void
UI::StartCountDown(unsigned long countDownPeriod_ms) 
{
  // UiEventTime eventTime;
UiEvent       event;

  m_timeEndTrigger_ms     = time.Now_ms() + countDownPeriod_ms;                   // event at time end  
  m_timeNearEndTrigger_ms = m_timeEndTrigger_ms - m_timeNearEndPeriod_ms - 1000;   // event x msec before time end to warn user - and add time = 0 for best user experience
  m_timeArmed_ms   = m_timeEndTrigger_ms + m_timeRearmingPeriod_ms;         // event x msec after time end to nudge user to leave
  m_timeUpdateTrigger_ms  = time.Now_ms();
  m_countingDown = true;
}

void
UI::StopCountDown() 
{
  m_countingDown = false;
}

unsigned long
UI::GetTimeRemaining(unsigned long time_ms) 
{
  return (m_countingDown) ? m_timeEndTrigger_ms - time_ms : 0;
}

void UI::CountDownService() 
{
  if (m_countingDown) {
    if (m_timeUpdateTrigger_ms < time.Now_ms()) {
      EventPush(TimeUpdate);
      m_timeUpdateTrigger_ms += 1000;
    }

    if (m_timeNearEndTrigger_ms < time.Now_ms()) {
      EventPush(TimeNearEnd);
      m_timeNearEndTrigger_ms = m_timeTriggerInfinite;
    }

    // if (m_timeEndTrigger_ms < time.Now_ms() - time.GetTimerInterval_ms()) {
    if (m_timeEndTrigger_ms < time.Now_ms()) {
      EventPush(TimeAtEnd);
      m_timeEndTrigger_ms = m_timeTriggerInfinite;
      m_timeUpdateTrigger_ms = m_timeTriggerInfinite;
    }

    if (m_timeArmed_ms < time.Now_ms()) {
      EventPush(TimeArmed);
      m_timeArmed_ms = m_timeTriggerInfinite;
    }
  }
}