//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <vector>
#include "device.h"
#include "constraint.h"

namespace ghpi {
    
    class Operator {
        
    private:
        vector<Device> devices_;
        vector<Constraint> constraints_;
    public:
        Device* GetDeviceByName(std::string dname);
        bool ToggleDevice(std::string dname);
        bool TurnOnDevice(std::string dname);
        bool TurnOffDevice(std::string dname);
        void RegisterDevice(Device* device);
        void RegisterConstraintForDevice(Constraint* constraint);
        bool CheckConstraints();
        Run();
        
    };
    
};