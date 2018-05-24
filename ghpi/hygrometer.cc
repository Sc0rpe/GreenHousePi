//
// 2018 Rico Schulz
//
#pragma once
#include "hygrometer.h"

std::map<std::string, void*> ghpi::Hygrometer::Run(void* env_var) {
  values_ = get_values();
  return values_;
}

std::vector<std::string, void*> ghpi::Hygrometer::get_values() {
  std::map<std::string, void*> values;
  //Read Soil Moisture
  int soil_mois = ReadSoilMoisture(CHANNEL);
  
  // add data to return map
  values["SM"] =  (void*)soil_mois;
  
  return values;
}

int ghpi::Hygrometer::ReadSoilMoisture(int channel) {
  int value = 0;
  // First turn on the sensor. This shall turn on the correct channel of the relay.
  TurnOn();
  
  // Read value from the A/D - converter channel
  GetValueFromChannel(channel);
  
  // Turn off to avoid electrolysis
  TurnOff();
  
  return value;
}