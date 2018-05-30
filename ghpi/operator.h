//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <vector>
#include <map>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_region.hpp>
#include "device.h"
#include "action.h"
#include "constraint.h"

#define MSG_QUEUE_SIZE 1000

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
      void RegisterConstraint(Constraint constraint);
      std::vector<Action> CheckConstraints(std::map<std::string, void*> values);
      void Run();
      void Clear();
      Operator();
    
     private:
      //Functions
      
      // Generating the appropiate action to meet violated constraint
      Action GenerateAction(ConstraintCondition condition);
      
      // Checks if the passed device is already registered
      //  ARGS:
      //      device - A Pointer to a Device which is checked for duplicates
      //  RETURN:
      //      true - If a duplicate was found
      //      false - If no duplicate was found
      bool CheckForDuplicateDevice(Device* device);
      
      // Data Members
      
      std::vector<Device> devices_;
      
      // Map of Constraints and the Actions to execute to meet them
      // Actions are expectet to be executet to meet the constraint values
      std::map<Constraint, Action> constraints_;
      
      struct shm_remove {
         shm_remove() { shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
      } remover_;
      
      boost::interprocess::shared_memory_object shm_messages_;
      boost::interprocess::mapped_region region_;
        
    };   
}