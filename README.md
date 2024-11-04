rc-sim-timer
===
Project to manage RC Flight Simulators like Reflex or Phoenix.

---
Features:

* Pushbutton to reset simulation when plane gets too far away - or crashes
* Countdown on 7 segment display to show remaining time
* Flashing of 7 segment display when time is running out
* Cool-down period (8-ish seconds) to nudge user to hand over controller to next person
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