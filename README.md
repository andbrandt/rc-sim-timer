rc-sim-timer
===
Project to manage RC Flight Simulators via an Arduino-based control panel.

Currently supported simulators:

* Phoenix v4
* RealFlight Basic
* RealFlight v9

---
# Features:

* Pushbutton to reset simulation when plane gets too far away - or crashes
* Countdown on 7 segment display to show remaining time
* Flashing of 7 segment display when time is running out
* Cool-down period (8-ish seconds) to nudge user to hand over controller to next person

---
# Simulator preparation
Depending on your simulator, you may have to do a litte setup to make it work with rc-sim-timer.
## Phoenix v4
Define two airplane favorites in the aircraft setup menu. Let the first (uppermost) be an electric high-wing beginner model and the second and aerobatic model like extra 232.

rc-sim-timer will choose an airplane from this list when you toggle between models using double press.
## RealFlight Basic
When you start the simulator, make sure to select the (???? - three lines down from "Krill Extra") airplane model.

rc-sim-timer will jump up and down between "?????" and "Krill Extra" when you toggle between models using double press.
## RealFlight v9
Define two airplane favorites in the "edited airplanes" section of the airplane setup menu. Let the first (uppermost) be an electric high-wing beginner model and the second and aerobatic model like extra 232.

rc-sim-timer will choose an aircraft from this list when you toggle between models using double press.


---
Required:

* Arduino with full USB mouse+keyboard HID support like Leonardo or Arduino Pro Micro
* Push button with built-in light for UI effects
* 4-digit 7-segment I2C display
---
**WARNING #1**

When connecting the Arduino Pro Micro, it may show up as "Arduino Leonardo" or "LilyPad Arduino USB".

**DO NOT** change the board type in the Arduino IDE. You may brick the device (in a soft way),requiring you to reset the bootloader by double-resetting and loading an empty sketch to the device. 

**WARNING #2**

The push button (two individual items) used in this project was found to have + and - swapped...
![Schematic](hardware/schematic.png)