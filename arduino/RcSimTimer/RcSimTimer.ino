/*
  RC Sim Timer
*/

#include <Mouse.h>
#include <Keyboard.h>
#include <Arduino.h>

#include "Debug.hpp"
#include "hal.hpp"
#include "Display7Seg.hpp"
#include "LED.hpp"
#include "LedPushButton.hpp"
#include "UiEvent.hpp"
#include "UI.hpp"
#include "src/anduinolib/io/usb-hid/KeyboardCtl.hpp"
#include "src/anduinolib/io/usb-hid/MouseCtl.hpp"
#include "src/anduinolib/sys/Time.hpp"

Display7Seg   display7Seg(DISPLAY_7SEG_PIN_CLK, 
                          DISPLAY_7SEG_PIN_SDA, 
                          DISPLAY_7SEG_LIGHT_LEVEL_OFF, 
                          DISPLAY_7SEG_LIGHT_LEVEL_ON);

LED           ledPcb(     ONBOARD_LED_PIN, 
                          ONBOARD_LED_LEVEL_OFF, 
                          ONBOARD_LED_LEVEL_ON);

LedPushButton ledPushButton(PUSHBUTTON_PIN_LED, 
                          PUSHBUTTON_LED_LEVEL_OFF, 
                          PUSHBUTTON_LED_LEVEL_ON, 
                          PUSHBUTTON_PIN_SWITCH);
UI            ui;

using namespace anduinolib::sys;
using namespace anduinolib::io::usbHid;

void setup() 
{
  mouseCtl.Begin();

  DEBUG_SETUP();
  time.Begin();

  display7Seg.Begin();
  ledPcb.Blink(true, 250, 250);
  ledPushButton.Begin(&ui, UiEvent::Setup, UiEvent::Enter, UiEvent::ModelToggle);
  ui.Begin(&display7Seg, &ledPushButton);
}

void loop() 
{
  time.Poll();
  ui.Poll();
  display7Seg.Poll();
  ledPcb.Poll();
  ledPushButton.Poll();
}