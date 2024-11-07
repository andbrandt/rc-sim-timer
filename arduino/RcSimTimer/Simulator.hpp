#ifndef Simulator_h
#define Simulator_h

#include <Arduino.h> 
// #include <Mouse.h>
// #include <Keyboard.h>

class Simulator {
  public:
    virtual ~Simulator() = default;

    virtual void Restart();
    virtual void Block();
    virtual void UnBlock();
    virtual void EasyAircraft();
    virtual void AcroAircraft();
};
#endif