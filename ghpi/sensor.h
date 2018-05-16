//
// 2018 Rico Schulz
//
#pragma once
#include "device.h"


namespace ghpi {
  
    class Sensor: public Device {
      public:
      // Functions
      std::vector<void*> get_values();
      
      private:
        // Data Member
        std::vector<void*> values_;
        std::vector<std::string> value_names;
    };
};