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
      std::map<std::string, float> Run(void* env_var) override;
      void RegisterAction(Action action);
      std::vector<Action> GetActionsByName(std::string name) const;
      bool CanExecute(std::string action_name) const;
      virtual void ExecuteAction(Action action);
      void Print() override;
      Actuator(std::string name);
      Actuator();
      ~Actuator();
      
      protected:
        // Data Member
        std::vector<Action> actions_;
    };
};