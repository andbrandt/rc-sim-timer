#ifndef SimulatorRealFlightBasic_h
#define SimulatorRealFlightBasic_h

#include "Simulator.hpp"

class SimulatorRealFlightBasic:public Simulator {
  public:
    void Restart();
    void Block();
    void UnBlock();
    void EasyAircraft();
    void AcroAircraft();
};
#endif