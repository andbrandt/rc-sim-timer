// #include "Arduino.h"

#include <Mouse.h>
#include <Keyboard.h>
#include "SimulatorRealFlightBasic.hpp"

void SimulatorRealFlightBasic::Restart() 
{
  // Always start unblocking - in case sim is currently blocked
  UnBlock();

  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
}

void SimulatorRealFlightBasic::Block() 
{
  // Always start unblocking - in case sim is currently blocked
  UnBlock();

  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_RIGHT_ARROW);
}

void SimulatorRealFlightBasic::UnBlock() 
{
  Keyboard.write(KEY_LEFT_ARROW);
  delay(10);
  Keyboard.write(KEY_LEFT_ARROW);
  delay(10);
}

void SimulatorRealFlightBasic::EasyAircraft() 
{
  // Always start unblocking - in case sim is currently blocked
  UnBlock();

  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_RIGHT_ARROW);
}

void SimulatorRealFlightBasic::AcroAircraft() 
{
  // Always start unblocking - in case sim is currently blocked
  UnBlock();

  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_UP_ARROW);
  delay(10);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(10);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(10);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(10);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(10);
  Keyboard.write(KEY_RIGHT_ARROW);
}
