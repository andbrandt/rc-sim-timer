#ifndef UI_H
#define UI_H

#include <Arduino.h> 
#include <ArduinoQueue.h>
#include "Display7Seg.hpp"
#include "LedPushButton.hpp"
#include "Simulator.hpp"
#include "UiEvent.hpp"

class UI : public UiEvent {
  private:
    const char m_versionString[5] = " 041";

    enum UiState {
      SimulatorSelect=0,
      TimeSelect=1,
      SimArmed=2,
      SimRunning=3,
      SimEnding=4,
      SimReArming=5,
    };  

    enum UiAircraft {
      Easy,
      Acro,
      last_aircraft,
    };

    enum UiSimDuration {
      SimDuration_2_00,
      SimDuration_5_00,
      SimDuration_99_99,
      last_simDuration,
    };

    enum UiSimApp {
      SimApp_Phoenix,
      SimApp_Reflex,
      last_simApp,
    };

    UiState                   m_state;
    UiAircraft                m_aircraftSelection;
    UiSimDuration             m_simDurationSelection;
    UiSimApp                  m_simAppSelection;
    const char                m_simDurationSelectionString[3][5] = {"0200", "0500", "1000"};
    const unsigned long       m_simDuration_ms[3] = {120000, 300000, 600000};
    const char                m_simAppSelectionString[2][5] = {"PHOE", "refl"};
    Display7Seg               *m_display7Seg;
    LedPushButton             *m_ledPushButton;
    bool                      m_countingDown;
    Simulator                 *m_simulator;

// CountDown control data
    unsigned long m_timeEndPeriod_ms;                   // Period from start (after armed) to end when sim is paused - controlled in runtime
    const unsigned long m_timeNearEndPeriod_ms = 15000; // Period before end when display is blinking
    const unsigned long m_timeRearmingPeriod_ms = 5000; // Period that follows after sim is paused

    unsigned long m_timeEndTrigger_ms;                  // The time at which the sim is paused
    unsigned long m_timeNearEndTrigger_ms;              // The time at which the display starts blinking
    unsigned long m_timeArmed_ms;                       // The time at which the system is armed again
    unsigned long m_timeUpdateTrigger_ms;               // The time at which the remaining time text is updated (each second)

    void StartCountDown(unsigned long countDownPeriod_ms);
    void StopCountDown();
    unsigned long GetTimeRemaining(unsigned long time_ms);
    void CountDownService();
    void SelectSimAircraft();
    void EventService();

  public:
    Simulator               *m_simAppSelection_apps[2];

    UI();
    void Begin(Display7Seg *display7Seg, LedPushButton *ledPushButton);
    void Poll();
    void StateSet(UiState state);
};
#endif