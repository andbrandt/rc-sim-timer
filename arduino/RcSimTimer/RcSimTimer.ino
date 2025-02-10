//#define DEBUG
#include "src/anduinolib/debug/Debug.hpp"

#include <Arduino.h>

#include "src/anduinolib/ui/Display7Seg.hpp"
#include "src/anduinolib/ui/LED.hpp"
#include "src/anduinolib/ui/LedPushButton.hpp"
#include "src/anduinolib/io/usb-hid/KeyboardCtl.hpp"
//#include "src/anduinolib/io/usb-hid/MouseCtl.hpp"
#include "src/anduinolib/sys/Time.hpp"

#include "hal.hpp"
#include "UiEvent.hpp"
#include "UI.hpp"

using namespace anduinolib::sys;
using namespace anduinolib::io::usbHid;
using namespace anduinolib::ui;

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

void setup() 
{
  bool safeMode = true;

  DEBUG_SETUP();  // <- Uncomment #define DEBUG in first line of this file to enable debug printing in ALL project files
  time.Begin();

  display7Seg.Begin();
  ledPcb.Blink(true, 250, 250);

  safeMode = ledPushButton.Begin(&ui, UiEvent::Setup, UiEvent::Enter, UiEvent::ModelToggle);

  if (safeMode) {
    DEBUG_PRINT("SafeMode");  
  }
  KeyboardCtl::Begin(!safeMode);
  ui.Begin(&display7Seg, &ledPushButton, safeMode);
}

void loop() 
{
  time.Poll();
  ui.Poll();
  display7Seg.Poll();
  ledPcb.Poll();
  ledPushButton.Poll();
}