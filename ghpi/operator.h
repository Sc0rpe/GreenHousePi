//
// 2018 Rico Schulz
//
#pragma once
#include <thread>
#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "environmentvalue.h"
#include "device.h"
#include "action.h"
#include "actuator.h"
#include "constraint.h"
#include "lcddisplay.h"
#include "debug.h"

#define MSG_QUEUE_SIZE 300

using namespace boost::interprocess;

// Injecting a custom hash function for our unordered_map of constraints
namespace std {
    template <>
    struct hash<ghpi::Constraint>
    {
      std::size_t operator()(const ghpi::Constraint& c) const
      {
        using std::size_t;
        using std::hash;
        using std::string;

        // Compute individual hash values for first,
        // second and third and combine them using XOR
        // and bit shifting:

        return (hash<string>()(c.get_name()));
      }
    };  
}

namespace ghpi {
  
    class Operator {
     public:
      // Functions
      Device* GetDeviceByName(std::string dname);
      std::vector<Device*> GetDevicesByType(DeviceType dtype);
      bool ToggleDevice(std::string dname);
      bool TurnOnDevice(std::string dname);
      bool TurnOffDevice(std::string dname);
      void RegisterDevice(Device *device);
      void RegisterConstraint(Constraint constraint, Action action);
      std::vector<Action> CheckConstraints(std::map<std::string, float> values);
      void Run();
      static void RefreshDisplay(const std::map<std::string, float> &values, LCDDisplay &disp);
      void PrintDevices();
      void PrintValues();
      void PrintConstraints();
      void Clear();
      void Set_LCDDisplay(LCDDisplay* display);
      Operator();
      ~Operator();
      
      static constexpr const char* const SHM_NAME = "GHPI_Messages";
      
      struct shm_remove {
        shm_remove() { shared_memory_object::remove(SHM_NAME); }
        ~shm_remove(){ shared_memory_object::remove(SHM_NAME); }
      } remover_;
      
      struct MSGQueue {
        Action actions_[256];
        int count;
        MSGQueue() { count = 0; }
        
        interprocess_mutex mutex;
        
        Action Pop() {
          --count;
          return actions_[count];
        }
        
        void Put(Action act) {
          actions_[count] = act;
          ++count;
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
      
      LCDDisplay* display_;
      std::vector<Device*> devices_;
      std::map<std::string, float> values_;
      
      // Map of Constraints and the Actions to execute to meet them
      // Actions are expectet to help meeting the constraint when executet
      //std::map<Constraint, Action> constraints_;
      std::unordered_map<Constraint, std::vector<Action>> constraints_;
 
      
      boost::interprocess::shared_memory_object shm_messages_;
      boost::interprocess::mapped_region region_;
        
    };   
}
