//
// 2018 Rico Schulz
//
// This class is for the SODIAL Hygrometer that measeures the
// soil moisture. To avoid electrolysis turn off the sensors power
// supply after measuring (Relay). Use Device::TurnOn() and Device::TurnOff() 
#pragma once
#include "analogsensor.h"

namespace ghpi {

  class Hygrometer : public AnalogSensor {
   public:
    // Functions
    std::map<std::string, float> Run(void* env_var) override;
    std::map<std::string, float> get_values() override;
    int ReadSoilMoisture(int channel);
    Hygrometer(ADConverter *adconverter, int  channel);
    ~Hygrometer();
   
   private:
  
  };
}