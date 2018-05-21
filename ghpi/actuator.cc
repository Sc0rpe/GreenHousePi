//
// 2018 Rico Schulz
//
#include "actuator.h"

void ExecuteAction(Action action) {
  switch(action.get_action_fn()) {
    case "ON": {
      TurnOn();
      break;
    }
    case "OFF": {
      TurnOff();
      break;
    }
    case "TOGGLE": {
      Toggle();
      break;
    }
    case "RUN": {
      Run();
      break;
    }
    default: {
      assert(false);
    }
  }
}

// Overriding Device::Run(void*)
void ghpi::Actuator::Run(void* env_var) {
  std::string s = static_cast<std::string>(*env_var); 
  std::vector<ghpi::Action> actions = GetActionsByName(s);
  
  for (auto &it: actions) {
    // Execute actions
    ExecuteAction(it);
  }
}

std::vector<ghpi::Action> ghpi::Actuator::GetActionsByName(std::string name) {
  std::vector<ghpi::Action> actions;
  
  for (auto &it: actions_) {
    if (it->get_name() == name)
      actions.push_back(it);
  }
  
  return actions;
}