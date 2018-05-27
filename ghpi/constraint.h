//
// 2018 Rico Schulz
//
#pragma once

namespace ghpi {
  
  enum ConstraintCondition {
    BELOW = 0,
    OVER ,
    EQUAL ,
  };
  static const char * ConstraintConditionStrings[] = {"BELOW", "OVER", "EQUAL"};
  
  class Constraint {
   public:
    // Functions
    bool CheckForValue(float value);
    std::string get_name();
    std::string get_variable();
    ConstraintCondition get_condition();
    // Constructors
    Constraint(std::string name, std::string variable, ConstraintCondition condition);
    Constraint(std::string name, std::string variable, std::string condition);
    
   private:
    // Data Members
    std::string name_;
    std::string variable_;
    ConstraintCondition condition_;
    float value_;
  };
};