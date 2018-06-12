//
// 2018 Rico Schulz
//
#include "ldr.h"

std::map<std::string, float> ghpi::LDR::get_values() {
  std::map<std::string, float> values; 
  int val = adconverter_->GetValueFromChannel(channel_);
  values["LIGHT_INTENSITY"] = (float)val;
  return values_;
}

ghpi::LDR::LDR(ghpi::ADConverter *adconverter) : AnalogSensor(adconverter) {
  name_ = "LDR";
  mode_ = OperationMode::AUTONOMOUS;
}

ghpi::LDR::~LDR() {
}
