/*
  RC Sim Timer
*/

#include <Mouse.h>
#include <Keyboard.h>
#include <Arduino.h>

#include "hal.hpp"
#include "Display7Seg.hpp"
#include "LED.hpp"
#include "LedPushButton.hpp"
#include "UI.hpp"
#include "Time.hpp"

Display7Seg display7Seg;
LED ledPcb;
LedPushButton ledPushButton;
UI ui;

void setup() {
/* ######################################################################################### */  
  // unsigned long bootBackdoor = 512;
  // Serial.begin(9600);

  // // while ((!Serial) && (--bootBackdoor)) {
  // while ((--bootBackdoor)) {
  // ; // wait for serial port to connect. Needed for Serial Port to work from first line of code.
  // }
  // delay(1000);
  // Serial.println(bootBackdoor);
  // Serial.println("WARNING: Code waits for Serial USB device to be ready for printing from first line.");
  // Serial.println("         Disable this code when bebugging is finished");
/* ######################################################################################### */
  
  time.Begin();

  display7Seg.Begin( DISPLAY_7SEG_PIN_CLK, DISPLAY_7SEG_PIN_SDA, DISPLAY_7SEG_LIGHT_LEVEL_OFF, DISPLAY_7SEG_LIGHT_LEVEL_ON);

  ledPcb.Begin(ONBOARD_LED_PIN, ONBOARD_LED_LEVEL_OFF, ONBOARD_LED_LEVEL_ON);
  ledPcb.Blink(true, 250, 250); 

  ledPushButton.Begin(PUSHBUTTON_PIN_LED, ONBOARD_LED_LEVEL_OFF, ONBOARD_LED_LEVEL_ON, PUSHBUTTON_PIN_SWITCH, &HandleButtonEvent);

  ui.Begin(&display7Seg, &ledPushButton);
}

void loop() {
  time.Poll();
  display7Seg.Poll();
  ledPcb.Poll();
  ledPushButton.Poll();
  ui.Poll();
}

void HandleButtonEvent(LedPushButton::ButtonEvent event) {
  switch (event) {
    case PushButton::LONG_PRESS:
      ui.EventPush(UI::Setup);
      break;
    case PushButton::SHORT_PRESS:
      ui.EventPush(UI::Enter);
      break;
    default:
        break;
  }
}