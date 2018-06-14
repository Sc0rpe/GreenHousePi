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

ghpi::Constraint::Constraint(std::string name, std::string variable, ConstraintCondition condition) {
  name_ = name;
  variable_ = variable;
  condition_ = condition;
}

ghpi::Constraint::Constraint(std::string name, std::string variable, std::string condition) {
  ConstraintCondition c;
  
  if (condition == "BELOW")
    c = ConstraintCondition::BELOW;
  else if (condition == "OVER")
    c = ConstraintCondition::OVER;
  else if (condition == "EQUAL")
    c = ConstraintCondition::EQUAL;
  else
    assert(false);
  
  Constraint(name, variable, c);
}

ghpi::Constraint::~Constraint() {
}

bool ghpi::Constraint::operator<(const Constraint &c2) const {
  return true;
}