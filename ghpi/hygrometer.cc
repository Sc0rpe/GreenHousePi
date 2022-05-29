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
  values[EnvironmentValueStrings[EnvironmentValue::SOIL_MOISTURE]] =  (float)soil_mois;
  
  return values;
}

float ghpi::Hygrometer::ReadSoilMoisture(int channel) {
  float value = 0;
  // First turn on the sensor. This should turn on the correct channel of the relay.
  TurnOn();
  
  // Read value from the A/D - converter channel
  for (int i = 0; i < HYG_MEASUREMENTS; ++i) {
    if (is_transform_linear)
      value += TransformLinear(adconverter_->GetValueFromChannel(channel));
    else
      value += adconverter_->GetValueFromChannel(channel);
  }
  
  value = value / HYG_MEASUREMENTS;
  
  // Turn off to avoid electrolysis
  TurnOff();
  
  return value;
}

float ghpi::Hygrometer::TransformLinear(float value) {
  float M = (max_.Y - min_.Y) / (max_.X - min_.X);
  float N = max_.Y - M * max_.X;
  return (M * value + N);
}

ghpi::Hygrometer::Hygrometer(ADConverter *adconverter, int channel) : AnalogSensor(adconverter, channel) {
  name_ = "Hygrometer_" + std::to_string(get_count());
  mode_ = OperationMode::AUTONOMOUS;
  channel_ = channel;
  is_transform_linear = false;
}

ghpi::Hygrometer::~Hygrometer() {
}