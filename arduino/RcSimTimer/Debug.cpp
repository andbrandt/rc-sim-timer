#include "Debug.hpp"

#ifdef DEBUG
void DebugSetup() 
{
  Serial.begin(9600);

  while (!Serial) {
  ; // wait for serial port to connect. Needed for Serial Port to work from first line of code.
  }
  delay(1000);
  Serial.println("WARNING: Code waits for Serial USB device to be ready for printing from first line.");
  Serial.println("         Disable this code when bebugging is finished");
}
#endif