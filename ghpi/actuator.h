//
// 2018 Rico Schulz
//
#pragma once
#include "device.h"


namespace ghpi {
  
    class Actuator: public Device {
      public:
      // Functions
      
      
      private:
        // Data Member
        std::vector<std::string> actions_;
    };
};