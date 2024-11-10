#ifndef DEBUG_h
#define DEBUG_h

#include <arduino.h>

#define DEBUG 1

  #ifdef DEBUG
    #define DEBUG_SETUP()         {void DebugSetup();}

    #define DEBUG_PRINT(f_, ...)  {Serial.print("DBG: ");              \
                                   Serial.print((f_), ##__VA_ARGS__);  \
                                   Serial.print("\n");                 \
                                  }

  #else
      #define DEBUG_SETUP()
      #define DEBUG_PRINT(...)
  #endif

#endif