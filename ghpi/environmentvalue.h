#pragma once
namespace ghpi {
  enum EnvironmentValue {
    TEMPERATURE = 0 ,
    HUMIDITY ,
    SOIL_MOISTURE ,
    LIGHT_INTENSITY ,
  };
  static const char * EnvironmentValueStrings[] = {"TEMP", "HUM", "SM", "LI"};
}