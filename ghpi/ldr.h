//
// 2018 Rico Schulz
//
#include "sensor.h"
#include "analogsensor.h"

namespace ghpi {

  // LDR stands for light dependent resistor aka photoresistor
  class LDR : public AnalogSensor {
   public:
    // Functions
    std::map<std::string, float> get_values() override;
    LDR(ADConverter *adconverter);
    ~LDR();
    
   private:
    // Functions
    
    // Data Members
    
  };
}