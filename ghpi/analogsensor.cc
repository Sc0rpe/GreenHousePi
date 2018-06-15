//
// 2018 Rico Schulz
//
#include "analogsensor.h"

ghpi::AnalogSensor::AnalogSensor(ghpi::ADConverter *adconverter, int channel) : Sensor() {
  name_ = "AnalogSensor_" + std::to_string(get_count());
  mode_ = OperationMode::AUTONOMOUS;
  channel_ = channel;
  adconverter_ = adconverter;
}

ghpi::AnalogSensor::~AnalogSensor() {
}