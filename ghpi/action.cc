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

void* ghpi::Action::get_additional_data() {
  return additional_data_;
}

void ghpi::Action::Print() {
  std::cout << "Action{" << name_ << "," << ActionFnStrings[action_fn_] << "}" ;
}

Action::Action(std::string name, ActionFn action_fn, void* add_data) {
  name_ = name;
  action_fn_ = action_fn;
  additional_data_ = add_data;
}