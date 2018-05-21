//
// 2018 Rico Schulz
//
#pragma  once
#include <iostream>

namespace ghpi {
  
  class Action {
   public:
    // Functions
    std::string get_name();
    std::string get_action_fn();
    void Print();
    Action(std::string, std::string);
    
   private:
    // Data Members
    
    // An arbitrary name for the action.
    // Name should match an Action in the list of 
    // actions of an actuator so it can be executed by it
    std::string name_;
    
    // Action FNs can be out of {RUN, ON, OFF, TOGGLE}.
    // These are the actions that can be executet by each device
    std::string action_fn_;  
  };
  
};