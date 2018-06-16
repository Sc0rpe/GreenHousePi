//
// 2018 Rico Schulz
//
#pragma once
#include "actuator.h"

namespace ghpi {

    class Pump: public Actuator {
     public:
      // Functions
      void ExecuteAction(Action action) override;
      Pump();
      ~Pump();
      
     private:
     // Functions
     
     // Waters given amount
     //   ARGS: 
     //     amount: amount of water in millitlitres
     void Water(float amount);
     
     void Print();
     
     // Data Members
     float ml_per_sec_;
    };
}