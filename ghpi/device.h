//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <vector>
#include "pin.h"

namespace ghpi {

    enum OperationMode {
        MANUALY = 0 , 
        AUTONOMOUS = 1
    };
    
    enum DeviceType {
        SENSOR = 0 ,
        AKTUATOR ,
        SWITCH ,
        RELAY
    };

    class Device {
    
    public:
        std::string name_;
        vector<Pin> pins_;
        OperationMode mode_;
        DeviceType type_;
    private:
        void RegisterPin(Pin* pin);
        void TurnOn();
        void TurnOff();
        void Toggle();
        
    
    };
};