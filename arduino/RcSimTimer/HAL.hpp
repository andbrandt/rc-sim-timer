#ifndef HAL_h
#define HAL_h

#include "debug.hpp"

// Defines for PCB
#define ONBOARD_LED_PIN 17
#define ONBOARD_LED_LEVEL_ON 0
#define ONBOARD_LED_LEVEL_OFF 255

// Defines for 7 segment display
#define DISPLAY_7SEG_PIN_CLK 4
#define DISPLAY_7SEG_PIN_SDA 5
#define DISPLAY_7SEG_LIGHT_LEVEL_ON 33
#define DISPLAY_7SEG_LIGHT_LEVEL_OFF 0

// Defines for pushbutton w. builtin LED
#define PUSHBUTTON_PIN_SWITCH 14
#define PUSHBUTTON_PIN_LED    10
#define ONBOARD_LED_LEVEL_ON  255
#define ONBOARD_LED_LEVEL_OFF 0

// Defines for joystick
#define JOYSTICK_PIN_BUTTON   PD6
#define JOYSTICK_PIN_AXIS_X   A0
#define JOYSTICK_PIN_AXIS_Y   A1

#endif