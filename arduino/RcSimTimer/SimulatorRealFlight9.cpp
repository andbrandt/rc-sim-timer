// #include "Arduino.h"

#include <Mouse.h>
#include <Keyboard.h>
#include "SimulatorRealFlight9.hpp"

void SimulatorRealFlight9::Restart() 
{
  // Always start unblocking - in case sim is currently blocked
  UnBlock();

  Keyboard.press(' ');
  delay(10);
  Keyboard.release(' ');
}

void SimulatorRealFlight9::Block() 
{
  // Always start unblocking - in case sim is currently blocked
  UnBlock();

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('a');
  Keyboard.release('a');
  Keyboard.press('a');
  Keyboard.release('a');
  Keyboard.release(KEY_LEFT_ALT);  
}

void SimulatorRealFlight9::UnBlock() 
{
  Keyboard.press(KEY_ESC);
  delay(50);
  Keyboard.write(KEY_ESC);
  delay(50);
}

void SimulatorRealFlight9::EasyAircraft() 
{
  // Always start unblocking - in case sim is currently blocked
  UnBlock();

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('a');
  Keyboard.release('a');
  Keyboard.press('a');
  Keyboard.release('a');
  Keyboard.release(KEY_LEFT_ALT);  

  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_LEFT_ARROW);
  delay(50);
  Keyboard.write(KEY_LEFT_ARROW);
  delay(50);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(50);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(50);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(500);

  Keyboard.write(KEY_RETURN);
}

void SimulatorRealFlight9::AcroAircraft() 
{
   // Always start unblocking - in case sim is currently blocked
  UnBlock();

 Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('a');
  Keyboard.release('a');
  Keyboard.press('a');
  Keyboard.release('a');
  Keyboard.release(KEY_LEFT_ALT);  

  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_LEFT_ARROW);
  delay(50);
  Keyboard.write(KEY_LEFT_ARROW);
  delay(50);
  Keyboard.write(KEY_RIGHT_ARROW);
  delay(50);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(500);

  Keyboard.write(KEY_RETURN);
}