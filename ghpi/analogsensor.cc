//
// 2018 Rico Schulz
//
#include "analogsensor.h"

AnalogSensor() : Sensor() {
  name_ = "AnalogSensor";
  mode_ = OperationMode::AUTONOMOUS;
}

~AnalogSensor() : ~Sensor() {
}