//
// 2018 Rico Schulz
//
#pragma once
#include "device.h"


namespace ghpi {
  
    class Sensor: public Device {
     public:
      // Functions
        virtual std::vector<void*> get_values();
        void Print();
      
     private:
      // Data Member
      std::map<std::string, void*> values_;
    };
};