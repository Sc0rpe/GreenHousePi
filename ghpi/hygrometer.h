//
// 2018 Rico Schulz
//
// This class is for the SODIAL Hygrometer that measeures the
// soil moisture. To avoid electrolysis turn off the sensors power
// supply after measuring (Relay). Use Device::TurnOn() and Device::TurnOff() 
#pragma once
#include "sensor.h"

namespace ghpi {

  class Hygrometer : public AnalogSensor {
   public:
    // Functions
    std::map<std::string, void*> Run(void* env_var);
    std::vector<std::string, void*> get_values();
    
    int ReadSoilMoisture(int channel);
   
   private:
  
  };
};