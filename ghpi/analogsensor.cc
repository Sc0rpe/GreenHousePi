//
// 2018 Rico Schulz
//
#include "analogsensor.h"

ghpi::AnalogSensor::AnalogSensor(ghpi::ADConverter *adconverter) : Sensor() {
  name_ = "AnalogSensor";
  mode_ = OperationMode::AUTONOMOUS;
  adconverter_ = adconverter;
}

ghpi::AnalogSensor::~AnalogSensor() {
}