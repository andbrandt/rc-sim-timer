// #include "Arduino.h"

#include <Mouse.h>
#include <Keyboard.h>
#include "SimulatorPhoenix.hpp"

void SimulatorPhoenix::Restart() 
{
  Keyboard.write('b');
}

void SimulatorPhoenix::Block() 
{
  Keyboard.press(KEY_RIGHT_CTRL);
  Keyboard.press('m');
  Keyboard.release('m');
  Keyboard.release(KEY_RIGHT_CTRL);  
}

void SimulatorPhoenix::UnBlock() 
{
  Keyboard.write(KEY_ESC);
}

void SimulatorPhoenix::EasyAircraft() 
{
  Keyboard.press(KEY_RIGHT_CTRL);
  Keyboard.press('m');
  Keyboard.release('m');
  Keyboard.release(KEY_RIGHT_CTRL);  

  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_DOWN_ARROW);

  Keyboard.write(KEY_RETURN);
}

void SimulatorPhoenix::AcroAircraft() 
{
  Keyboard.press(KEY_RIGHT_CTRL);
  Keyboard.press('m');
  Keyboard.release('m');
  Keyboard.release(KEY_RIGHT_CTRL);  

  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_UP_ARROW);
  delay(50);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(50);
  Keyboard.write(KEY_DOWN_ARROW);
  delay(50);

  Keyboard.write(KEY_RETURN);
}