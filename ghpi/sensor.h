//
// 2018 Rico Schulz
//
#pragma once
#include "device.h"


namespace ghpi {
  
    class Sensor: public Device {
     public:
      // Functions
      std::map<std::string, float> Run(void* env_var) override;
      virtual std::map<std::string, float> get_values() = 0;
      void Print();
      Sensor();
      ~Sensor();
      
     protected:
      // Data Member
      std::map<std::string, float> values_;
    };
}