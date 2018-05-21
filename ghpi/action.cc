//
// 2018 Rico Schulz
//
#include "action.h"

std::string ghpi::Action::get_name() {
  return name_;
}

std::string ghpi::Action::get_action_fn() {
  return action_fn_;
}

void ghpi::Action::Print() {
  std::cout << "Action{" << name_ << "," << action_fn_ << "}" ;
}

Action::Action(std::string name, std::string action_fn) {
  name_ = name;
  action_fn_ = action_fn;
}