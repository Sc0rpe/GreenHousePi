//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <cassert>
#include <iostream>
#include "debug.h"

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
    
    // Checks if the condition has been met for a given value
    //  ARGS: 
    //    value to check for
    //  RETURNS:
    //    True - If the condition has been met.
    //    False - Else.
    bool CheckForValue(float value) const;
    
    std::string get_name() const;
    std::string get_variable() const;
    ConstraintCondition get_condition();
    // Constructors
    Constraint(std::string name, std::string variable, float val, ConstraintCondition condition);
    Constraint(std::string name, std::string variable, float val, std::string condition);
    Constraint(std::string name, std::string variable, float val);
    ~Constraint();
    
    // Operators
    bool operator<(const Constraint &c2) const;
    bool operator==(const Constraint &c2) const;
    
   private:
    // Data Members
    std::string name_;
    std::string variable_;
    ConstraintCondition condition_;
    float value_;
  };
};