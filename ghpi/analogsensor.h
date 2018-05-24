//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"

namespace ghpi {
  
  class AnalogSensor : public Sensor {
   public:
    int GetValueFromChannel(int channel);
    
   private:
    ADConverter adconverter_;
    
  }
  
}