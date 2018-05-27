//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"

std::map*<std::string, void*> ghpi::Sensor::Run(void* env_var) {
  values_.clear();
  values_ = get_values();
  return &values_;
}

void ghpi::Sensor::Print() {
  std::cout << "Sensor{";
  device::Print();
  for (auto &it: values_) {
    std::cout << "," << it->first;
  }
  std::cout << "}";
}