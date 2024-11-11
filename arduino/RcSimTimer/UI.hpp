#ifndef UI_H
#define UI_H

#include <Arduino.h> 
#include <ArduinoQueue.h>
#include "EEPROM.h"
#include "Display7Seg.hpp"
#include "LedPushButton.hpp"
#include "Simulator.hpp"
#include "UiEvent.hpp"
#include "Debug.hpp"

class UI : public UiEvent {
  private:
    const char m_versionString[5] = "v085";

    enum UiState {
      SimulatorSelect,
      TimeSelect,
      SimArmed,
      SimRunning,
      SimEnding,
      SimReArming,
    };  

    enum UiAircraft {
      Easy,
      Acro,
      Aircraft_last,
    };

    enum UiSimDuration {
      SimDuration_2_00,
      SimDuration_5_00,
      SimDuration_99_99,
      simDuration_last,
    };

    enum UiSimApp {
      SimApp_Phoenix,
      SimApp_RealFlight9,
      SimApp_RealFlightBasic,
      simApp_last,
    };

    struct UiSettings {
        unsigned char version=1;
        unsigned char structSize=5;
        unsigned char simDurationSelection;
        unsigned char simAppSelection;
        unsigned char chkSum;
    };

    UiState                   m_state;
    UiAircraft                m_aircraftSelection;
    // UiSimDuration             m_simDurationSelection;
    // UiSimApp                  m_simAppSelection;
    UiSettings                m_settings;
    const char                m_simDurationSelectionString[simDuration_last][5] = {"0200", "0500", "1000"};
#ifdef DEBUG
    const unsigned long       m_simDuration_ms[simDuration_last] = { 10000,  30000,  60000};
#else    
    const unsigned long       m_simDuration_ms[simDuration_last] = {120000, 300000, 600000};
#endif    
    Display7Seg               *m_display7Seg;
    LedPushButton             *m_ledPushButton;
    bool                      m_countingDown;
    Simulator                 *m_simulator;
    Simulator                 *m_simAppSelection_apps[simApp_last];
    const char                m_simAppSelectionString[simApp_last][5] = {"PHOE", "rf_9", "rf_b"};

// CountDown control data
    unsigned long m_timeEndPeriod_ms;                   // Period from start (after armed) to end when sim is paused - controlled in runtime
#ifdef DEBUG
    const unsigned long m_timeNearEndPeriod_ms  = 7000; // Period before end when display is blinking
    const unsigned long m_timeRearmingPeriod_ms = 2000; // Period that follows after sim is paused
#else
    const unsigned long m_timeNearEndPeriod_ms  = 15000; // 15000; // Period before end when display is blinking
    const unsigned long m_timeRearmingPeriod_ms = 5000; // Period that follows after sim is paused
#endif
    const unsigned long m_timeTriggerInfinite   = 0xffffffff;
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
    bool LoadSettings();
    void SaveSettings();
    void InitSettings();
    void CalcSettingsChkSum();

  public:

    UI();
    void Begin(Display7Seg *display7Seg, LedPushButton *ledPushButton);
    void Poll();
    void StateSet(UiState state);
};
#endif