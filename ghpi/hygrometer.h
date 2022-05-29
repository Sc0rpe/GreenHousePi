//
// 2018 Rico Schulz
//
// This class is for the SODIAL Hygrometer that measures the
// soil moisture. To avoid electrolysis turn off the sensors power
// supply after measuring (Relay). Use Device::TurnOn() and Device::TurnOff() 

#pragma once
#include "analogsensor.h"
#define HYG_MEASUREMENTS 10

namespace ghpi {

  struct Point2D {
    float X;
    float Y;
  };

  class Hygrometer : public AnalogSensor {
   public:
    // Functions
    bool is_transform_linear;
    Point2D min_;
    Point2D max_;
    std::map<std::string, float> Run(void* env_var) override;
    std::map<std::string, float> get_values() override;
    float ReadSoilMoisture(int channel);
    float TransformLinear(float value);
    Hygrometer(ADConverter *adconverter, int  channel);
    ~Hygrometer();
   
   private:
    
  
  };
}