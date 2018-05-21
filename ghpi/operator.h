//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <vector>
#include <map>
#include "device.h"
#include "constraint.h"

namespace ghpi {
    
    enum EnvironmentValue {
      TEMPERATURE = 0 ,
      HUMIDITY ,
      SOIL_MOISTURE ,
      LIGHT_INTENSITY ,
    };
    
    class Operator {
     public:
      // Functions
      Device* GetDeviceByName(std::string dname);
      std::vector<Device*> GetDevicesByType(DeviceType dtype);
      bool ToggleDevice(std::string dname);
      bool TurnOnDevice(std::string dname);
      bool TurnOffDevice(std::string dname);
      void RegisterDevice(Device* device);
      void RegisterConstraintForDevice(Constraint* constraint);
      bool CheckConstraints();
      void Run();
      void Clear();
		
     private:
      // Checks if the passed device is already registered
      //  ARGS:
      //      device - A Pointer to a Device which is checked for duplicates
      //  RETURN:
      //      true - If a duplicate was found
      //      false - If no duplicate was found
      bool CheckForDuplicateDevice(Device* device);
      
      // Data Members
      std::vector<Device> devices_;
      std::vector<Constraint> constraints_;
      std::map<Constraint*, Device*> device_constraints_;
        
    };
    
};