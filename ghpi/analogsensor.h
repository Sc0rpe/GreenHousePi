//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"
#include "adconverter.h"

namespace ghpi {
  
  class AnalogSensor : public Sensor {
   public:
    // Retrieves the value of given channel as an int
    // in range from 0 to 1023 from the a/d converter
    int GetValueFromChannel(int channel);
    
   private:
    ADConverter adconverter_;
    int channel_;
    
  }
  
}