// #include "Arduino.h"

#include <Mouse.h>
#include <Keyboard.h>
#include "SimulatorReflex.hpp"

void SimulatorReflex::Restart() 
{
  Keyboard.write('r');
}

void SimulatorReflex::Block() 
{
  Keyboard.press(KEY_RIGHT_CTRL);
  Keyboard.press('m');
  Keyboard.release('m');
  Keyboard.release(KEY_RIGHT_CTRL);  
}

void SimulatorReflex::UnBlock() 
{
  Keyboard.write(KEY_ESC);
}

void SimulatorReflex::EasyAircraft() 
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

void SimulatorReflex::AcroAircraft() 
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