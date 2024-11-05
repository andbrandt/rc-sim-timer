#ifndef SimulatorReflex_h
#define SimulatorReflex_h

#include "Simulator.hpp"

class SimulatorReflex:public Simulator {
  public:
    void Reset();
    void Block();
    void UnBlock();
    void EasyAircraft();
    void AcroAircraft();
};
#endif