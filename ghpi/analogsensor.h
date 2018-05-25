//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"

namespace ghpi {
  
  class AnalogSensor : public Sensor {
   public:
    // Retrieves the value of given channef as an int
    // in range from 0 to 1023 from the a/d converter
    int GetValueFromChannel(int channel);
    
    float GetAsPercentageFromChannel(int channel);
    
   private:
    ADConverter adconverter_;
    
  }
  
}