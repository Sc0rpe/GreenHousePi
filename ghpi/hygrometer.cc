//
// 2018 Rico Schulz
//
#pragma once
#include "hygrometer.h"

std::map<std::string, float> ghpi::Hygrometer::Run(void* env_var) {
  values_ = get_values();
  return values_;
}

std::map<std::string, float> ghpi::Hygrometer::get_values() {
  std::map<std::string, float> values;
  //Read Soil Moisture
  int soil_mois = ReadSoilMoisture(channel_);
  
  // add data to return map
  values["SM"] =  (float)soil_mois;
  
  return values;
}

int ghpi::Hygrometer::ReadSoilMoisture(int channel) {
  int value = 0;
  // First turn on the sensor. This shall turn on the correct channel of the relay.
  TurnOn();
  
  // Read value from the A/D - converter channel
  value = adconverter_->GetValueFromChannel(channel);
  
  // Turn off to avoid electrolysis
  TurnOff();
  
  return value;
}

ghpi::Hygrometer::Hygrometer(ADConverter *adconverter) : AnalogSensor(adconverter) {
  name_ = "Hygrometer";
  mode_ = OperationMode::AUTONOMOUS;
}

ghpi::Hygrometer::~Hygrometer() {
}