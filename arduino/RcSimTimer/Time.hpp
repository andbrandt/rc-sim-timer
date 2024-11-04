#ifndef TIME_h
#define TIME_h

#include <Arduino.h> 
#include <arduino-timer.h>

class Time {
  public:
    static void Begin();
    static void Poll();
    static unsigned long Now_ms();
    static int GetTimerInterval_ms();

  private:
    static Timer<>::Task m_TimerTask;
    static Timer<10>     m_timer; // Timer with 10 task slots

    static unsigned long m_time_ms;
    static int           m_timerInterval_ms; // unit: 1ms

    static bool m_OnTimer25msec(void *argument);
};

extern Time time;

#endif