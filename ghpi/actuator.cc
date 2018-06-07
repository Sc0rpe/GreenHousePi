//
// 2018 Rico Schulz
//
#include "actuator.h"

void ExecuteAction(Action action) {
  switch(action.get_action_fn()) {
    case ActionFn::AFN_ON: {
      TurnOn();
      break;
    }
    case ActionFn::AFN_OFF: {
      TurnOff();
      break;
    }
    case ActionFn::AFN_TOGGLE: {
      Toggle();
      break;
    }
    case ActionFn::AFN_RUN: {
      Run();
      break;
    }
    default: {
      assert(false);
    }
  }
}

// Overriding Device::Run(void*)
std::map<std::string, float>* ghpi::Actuator::Run(void* env_var) {
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

void ghpi::Actuator::Print() {
  std::cout << "Actuator{";
  device::Print();
  for (auto &it: actions_) {
    it->Print();
    std::cout << std::endl;
  }
  std::cout << "}";
}

Actuator::Actuator(): Device() {
  name_ = "Actuator";
  mode_ = OperationMode::AUTONOMOUS;
}

Actuator::~Actuator(): ~Device() {
}