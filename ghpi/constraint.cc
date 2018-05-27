//
// 2018 Rico Schulz
//
#include "constraint.h"


bool ghpi::Constraint::CheckForValue(float value) {
  switch (condition_) {
    case ConstraintCondition::BELOW: {
      if (value > value_)
        return false;
      break;
    }
    case ConstraintCondition::OVER: {
      if (value < value_)
        return false;
      break;
    }
    case ConstraintCondition::EQUAL: {
      if (value != value_)
        return false;
      break;
    }
    default: {
      assert(false);
    }
  }
  return true;
}

std::string ghpi::Constraint::get_name() {
  return name_;
}

std::string ghpi::Constraint::get_variable() {
  return variable_;
}

ConstraintCondition ghpi::Constraint::get_condition() {
  return condition_;
}

ghpi::Constraint::Constraint(std::string name, std::string variable, ConstraintCondition condition) {
  name_ = name;
  variable_ = variable;
  condition_ = condition;
}

ghpi::Constraint::Constraint(std::string name, std::string variable, std::string condition) {
  ConstraintCondition c;
  
  switch (condition) {
    case "BELOW": {
      c = ConstraintCondition::BELOW;
      break;
    }
    case "OVER": {
      c = ConstraintCondition::OVER;
      break;
    }
    case "EQUAL": {
      c = ConstraintCondition::EQUAL;
      break;
    }
    default: {
      assert(false);
    }
  }
  Constraint(name, variable, c);
}