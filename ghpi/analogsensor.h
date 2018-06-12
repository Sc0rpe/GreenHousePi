//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"
#include "adconverter.h"

namespace ghpi {
  
  class AnalogSensor : public Sensor {
   public:
    AnalogSensor(ADConverter *adconverter);
    ~AnalogSensor();
   protected:
    ADConverter *adconverter_;
    int channel_;
    
  };
  
}