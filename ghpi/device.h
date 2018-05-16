//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <vector>
#include <map>
#include "pin.h"
#include <wiringPi.h>

namespace ghpi {

    enum OperationMode {
      MANUALLY = 0 , 
      AUTONOMOUS = 1
    };
    
    enum DeviceType {
      SENSOR = 0 ,
      AKTUATOR ,
      SWITCH ,
      RELAY
    };
    
    // This State describes whether the "switch pins"
    // must be high or low in order the device is turned on.
    enum OnState {
      LOW = 0 ,
      HIGH = 1
    };
    
    enum PinUsage {
      SWITCH = 0 ,
      BI_DATA ,
      MOSI ,
      MISO ,
      CLCK ,
      CS
    };
    
    enum DeviceState {
      OFF = 0 ,
      ON = 1
    };

    class Device { 
     public:  
      // Functions
      virtual void Run(void* env_var);
      virtual void TurnOn();
      virtual void TurnOff();
      virtual void Toggle();
      void RegisterPin(Pin* pin, PinUsage pin_usage, OnState on_state);
      std::string get_name();
      std::vector<Pin*> GetPinsByState(PinState state);
      std::vector<Pin*> GetPinsByMode(PinMode mode);
      std::vector<Pin*> GetPinsByUsage(PinUsage usage);
      std::string get_name();
      OperationMode get_mode();
      DeviceState get_state();
      DeviceType get_type();
      Device();
      
     private:
      // Data Members
      std::string name_;
      std::vector<Pin*> pins_;
      std::map<Pin*, PinUsage> pin_usages_;
      std::map<Pin*, OnState> pin_on_states_;
      OperationMode mode_;
      DeviceType type_;
      DeviceState state_;
    
    };
};