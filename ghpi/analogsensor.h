//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"
#include "adconverter.h"

namespace ghpi {
  
  class AnalogSensor : public Sensor {
   public:
    AnalogSensor(ADConverter *adconverter, int channel);
    ~AnalogSensor();
   protected:
    ADConverter *adconverter_;
    int channel_;
    
  };
  
}