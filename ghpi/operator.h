//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <vector>
#include <map>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "device.h"
#include "action.h"
#include "actuator.h"
#include "constraint.h"

#define MSG_QUEUE_SIZE 1000

using namespace boost::interprocess;

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
      void RegisterDevice(Device *device);
      void RegisterConstraint(Constraint constraint);
      std::vector<Action> CheckConstraints(std::map<std::string, float> values);
      void Run();
      void Clear();
      Operator();
      ~Operator();
      
      struct MSGQueue {
        Action actions_[256];
        int index_;
        MSGQueue() { index_ = 0; }
        
        Action Get(int i) {
          --index_;
          return actions_[i];
        }
        
        void Put(Action act) {
          actions_[index_] = act;
          ++index_;
        }
      };
    
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
      bool CheckForDuplicateDevice(Device *device);
      
      std::vector<ghpi::Action> ReadMessagesFromQueue();
      
      // Data Members
      
      std::vector<Device*> devices_;
      
      // Map of Constraints and the Actions to execute to meet them
      // Actions are expectet to be executet to meet the constraint values
      std::map<Constraint, Action> constraints_;
      
      static constexpr const char* const SHM_NAME = "GHPI_Messages";
      
      struct shm_remove {
        shm_remove() { shared_memory_object::remove(SHM_NAME); }
        ~shm_remove(){ shared_memory_object::remove(SHM_NAME); }
      } remover_;
    
      
      boost::interprocess::shared_memory_object shm_messages_;
      boost::interprocess::mapped_region region_;
        
    };   
}
