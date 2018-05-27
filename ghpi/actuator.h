//
// 2018 Rico Schulz
//
#pragma once
#include <assert.h>
#include "device.h"
#include "action.h"

namespace ghpi {
  
    class Actuator: public Device {
      public:
      // Functions
      std::map*<std::string, void*> Run(void* env_var);
      void RegisterAction(Action action);
      std::vector<Action> GetActionsByName(std::string name);
      virtual void ExecuteAction(Action action);
      void Print();
      
      private:
        // Data Member
        std::vector<Action> actions_;
    };
};