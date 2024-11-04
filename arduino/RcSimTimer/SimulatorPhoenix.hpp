#ifndef SimulatorPhoenix_h
#define SimulatorPhoenix_h

// #include <Arduino.h> 
// #include <Mouse.h>
// #include <Keyboard.h>
#include "Simulator.hpp"

class SimulatorPhoenix:public Simulator {
  public:
    void Reset();
    void Block();
    void UnBlock();
    void EasyAircraft();
    void AcroAircraft();
};
#endif