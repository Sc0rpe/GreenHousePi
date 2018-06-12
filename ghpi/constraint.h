//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <cassert>

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
    bool CheckForValue(float value) const;
    std::string get_name() const;
    std::string get_variable() const;
    ConstraintCondition get_condition();
    // Constructors
    Constraint(std::string name, std::string variable, ConstraintCondition condition);
    Constraint(std::string name, std::string variable, std::string condition);
    ~Constraint();
    
   private:
    // Data Members
    std::string name_;
    std::string variable_;
    ConstraintCondition condition_;
    float value_;
  };
};