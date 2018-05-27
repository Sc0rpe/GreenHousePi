//
// 2018 Rico Schulz
//
#include "ldr.h"

std::map<std::string, void*> get_values() {
  std::map<std::string, void*> values; 
  int val = adconverter_.GetValueFromChannel(channel_);
  values["LIGHT_INTENSITY"] = val;
  return values_;
}