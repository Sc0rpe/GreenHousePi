//
// 2018 Rico Schulz
//
#include "ldr.h"


std::map<std::string, float> ghpi::LDR::Run(void* env_var) {
  std::map<std::string, float> vals = get_values();
  return vals;
}

std::map<std::string, float> ghpi::LDR::get_values() {
  std::map<std::string, float> values; 
  int val = adconverter_->GetValueFromChannel(channel_);
  values[EnvironmentValueStrings[EnvironmentValue::LIGHT_INTENSITY]] = (float)val;
  return values;
}

ghpi::LDR::LDR(ghpi::ADConverter *adconverter, int channel) : AnalogSensor(adconverter, channel) {
  name_ = "LDR_" + std::to_string(get_count());;
  mode_ = OperationMode::AUTONOMOUS;
  channel_ = channel;
}

ghpi::LDR::~LDR() {
}
