//
// 2018 Rico Schulz
//
#include "actuator.h"

void ghpi::Actuator::ExecuteAction(Action action) {
  // execute the action if the device is operated autonomous or
  // if the action was created manually
	if (action.get_manually() || mode_ == ghpi::OperationMode::AUTONOMOUS) {
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
			case ActionFn::AFN_OP_MANU: {
				mode_ = OperationMode::MANUALLY;
				break;
			}
			case ActionFn::AFN_OP_AUTO: {
				mode_ = OperationMode::AUTONOMOUS;
				break;
			}
			default: {
				assert(false);
			}
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
  std::map<std::string, float> r;
  return r;
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

bool ghpi::Actuator::CanExecute(std::string action_name) const {
  for (auto it : actions_) {
    if (it.get_name() == action_name)
      return true;
  }
  return false;
}

void ghpi::Actuator::Print() {
  std::cout << "Actuator{";
  Device::Print();
  std::cout << std::endl;
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