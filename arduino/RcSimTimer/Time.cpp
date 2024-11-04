#include "Arduino.h"

#include "Time.hpp"

// Time time;
// Allocate storage as it is declared static
Timer<>::Task Time::m_TimerTask;
Timer<10>     Time::m_timer; // Timer with 10 task slots
unsigned long Time::m_time_ms;
int           Time::m_timerInterval_ms;

static unsigned long Time::Now_ms() {                     // unit: 1ms  <-  NOTE: NOT part of the Time class - may be called without class instantiation
  return m_time_ms;
}

static void Time::Begin() {
  m_timerInterval_ms = 25;
  m_timer.every(m_timerInterval_ms, m_OnTimer25msec);
}

static void Time::Poll() {      // time in ms.
    m_timer.tick();
}

static bool Time::m_OnTimer25msec(void *argument /* optional argument given to in/at/every */) {
  m_time_ms += m_timerInterval_ms;
  return true; // to repeat the action - false to stop
}

static int Time::GetTimerInterval_ms() {
  return m_timerInterval_ms;
}
