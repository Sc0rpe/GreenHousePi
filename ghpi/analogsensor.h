//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"
#include "adconverter.h"

namespace ghpi {
  
  class AnalogSensor : public Sensor {
   public:
    AnalogSensor();
    ~AnalogSensor();
    
   private:
    ADConverter adconverter_;
    int channel_;
    
  }
  
}