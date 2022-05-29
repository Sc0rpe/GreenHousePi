//
// 2018 Rico Schulz
//
#include "action.h"

const char* ghpi::Action::get_name() const {
  return name_;
}

ghpi::ActionFn ghpi::Action::get_action_fn() const {
  return action_fn_;
}

void* ghpi::Action::get_additional_data() {
  return additional_data_;
}

const char* ghpi::Action::get_target() const {
	return target_;
}

void ghpi::Action::set_manually(bool man) {
	manually_ = man;
}

bool ghpi::Action::get_manually() {
	return manually_; }

bool ghpi::Action::has_target() const {
  if (target_[0] == '\0')
    return false;
  return true;
}

void ghpi::Action::Print() {
  std::cout << "Action{" << name_ << "," << ActionFnStrings[static_cast<int>(action_fn_)] << "}" ;
}

ghpi::Action::Action(const char* name, ActionFn action_fn, void* add_data, const char* target, bool manually) {
  strcpy(name_, name);
  action_fn_ = action_fn;
  additional_data_ = add_data;
	strcpy(target_, target);
	manually_ = manually;
}

ghpi::Action::Action() {
  strcpy(name_, "Action");
  action_fn_ = ActionFn::AFN_OFF;
}

ghpi::Action::Action(const char* name, ActionFn action_fn, void* add_data) {
  strcpy(name_, name);
  action_fn_ = action_fn;
  additional_data_ = add_data;
	target_[0] = '\0';
	manually_ = false;
}

ghpi::Action::~Action() {

}

bool ghpi::Action::operator==(Action const &a2) const {
  if (strcmp(name_, a2.get_name()) == 0)
    return true;
  return false;
}