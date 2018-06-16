//
// 2018 Rico Schulz
//
#include "actuator.h"

void ghpi::Actuator::ExecuteAction(Action action) {
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
      Run(NULL);
      break;
    }
    default: {
      assert(false);
    }
  }
}

// Overriding Device::Run(void*)
std::map<std::string, float> ghpi::Actuator::Run(void* env_var) {
  //std::string s = static_cast<std::string>(*env_var); 
  //std::vector<ghpi::Action> actions = GetActionsByName(s);
  
  //for (auto &it: actions) {
    // Execute actions
  //  ExecuteAction(it);
  //}
}

void ghpi::Actuator::RegisterAction(Action action) {
  actions_.push_back(action);
}

std::vector<ghpi::Action> ghpi::Actuator::GetActionsByName(std::string name) const {
  std::vector<ghpi::Action> actions;
  
  for (auto const &it: actions_) {
    if (it.get_name() == name)
      actions.push_back(it);
  }
  return actions;
}

void ghpi::Actuator::Print() {
  std::cout << "Actuator{";
  Device::Print();
  for (auto &it: actions_) {
    it.Print();
    std::cout << std::endl;
  }
  std::cout << "}";
}

ghpi::Actuator::Actuator(std::string name): Device(name) {
  type_ = DeviceType::ACTUATOR;
  mode_ = OperationMode::AUTONOMOUS;
}

ghpi::Actuator::Actuator(): Device() {
  name_ = "Actuator_" + std::to_string(get_count());
  type_ = DeviceType::ACTUATOR;
  mode_ = OperationMode::AUTONOMOUS;
}

ghpi::Actuator::~Actuator() {
}