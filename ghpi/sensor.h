//
// 2018 Rico Schulz
//
#pragma once
#include "device.h"


namespace ghpi {
  
    class Sensor: public Device {
     public:
      // Functions
      std::map*<std::string, void*> Run(void* env_var);
      virtual std::map<std::string, void*> get_values() = 0;
      void Print();
      
     private:
      // Data Member
      std::map<std::string, void*> values_;
    };
}