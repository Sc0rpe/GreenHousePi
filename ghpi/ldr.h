//
// 2018 Rico Schulz
//
#include "sensor.h"

namespace ghpi {

  // LDR stands for light dependent resistor aka photoresistor
  class LDR: public AnalogSensor {
   public:
    // Functions
    std::map<std::string, void*> get_values();
    
   private:
    // Functions
    
    // Data Members
    
  };
}