#ifndef SimulatorRealFlight9_h
#define SimulatorRealFlight9_h

#include "Simulator.hpp"

class SimulatorRealFlight9:public Simulator {
  public:
    void Restart();
    void Block();
    void UnBlock();
    void EasyAircraft();
    void AcroAircraft();
};
#endif