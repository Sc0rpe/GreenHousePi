//
// 2018 Rico Schulz
//
#include "constraint.h"


bool ghpi::Constraint::CheckForValue(float value) const {
  switch (condition_) {
    case ConstraintCondition::BELOW: {
      if (value > value_) {
        #ifdef DEBUG
          std::cout << "[Constraint] Condition BELOW has NOT been met" << std::endl;
        #endif
        return false;
      }
      break;
    }
    case ConstraintCondition::OVER: {
      if (value < value_) {
        #ifdef DEBUG
          std::cout << "[Constraint] Condition OVER has NOT been met" << std::endl;
        #endif
        return false;
      }
      break;
    }
    case ConstraintCondition::EQUAL: {
      if (value != value_) {
        #ifdef DEBUG
          std::cout << "[Constraint] Condition EQUAL has NOT been met" << std::endl;
        #endif
        return false;
      }
      break;
    }
    default: {
      assert(false);
    }
  }
  #ifdef DEBUG
    std::cout << "[Constraint] Condition has been met" << std::endl;
  #endif
  return true;
}

std::string ghpi::Constraint::get_name() const {
  return name_;
}

std::string ghpi::Constraint::get_variable() const {
  return variable_;
}

ghpi::ConstraintCondition ghpi::Constraint::get_condition() {
  return condition_;
}

ghpi::Constraint::Constraint(std::string name, std::string variable, float val) {
  name_ = name;
  value_ = val;
  variable_ = variable;
  condition_ = ConstraintCondition::BELOW;
}

ghpi::Constraint::Constraint(std::string name, std::string variable, float val, ConstraintCondition condition) : Constraint(name, variable, val) {
  condition_ = condition;
}

ghpi::Constraint::Constraint(std::string name, std::string variable, float val, std::string condition) : Constraint(name, variable, val) {
  if (condition == "BELOW")
    condition_ = ConstraintCondition::BELOW;
  else if (condition == "OVER")
    condition_ = ConstraintCondition::OVER;
  else if (condition == "EQUAL")
    condition_ = ConstraintCondition::EQUAL;
  else
    assert(false);
}

ghpi::Constraint::~Constraint() {
}

bool ghpi::Constraint::operator==(const Constraint &c2) const {
  if (this->get_name() == c2.get_name())
    return true;
  return false;
}

bool ghpi::Constraint::operator<(const Constraint &c2) const {
  if (this->get_name() != c2.get_name())
    return true;
  return false;
}