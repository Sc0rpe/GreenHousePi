//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"

void ghpi::Sensor::Print() {
  std::cout << "Sensor{";
  device::Print();
  for (auto it: values_) {
    std::cout << "," << it->first;
  }
  std::cout << "}";
}