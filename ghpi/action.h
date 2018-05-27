//
// 2018 Rico Schulz
//
#pragma  once
#include <iostream>

namespace ghpi {
  
  enum ActionFn {
    OFF = 0 ,
    ON ,
    TOGGLE ,
    RUN
  };
  static const char * ActionFnStrings[] = {"OFF", "ON", "TOGGLE", "RUN"};
  
  class Action {
   public:
    // Functions
    std::string get_name();
    std::string get_action_fn();
    void Print();
    Action(std::string name, ActionFn action_fn);
    
   private:
    // Data Members
    
    // An arbitrary name for the action.
    // Name should match an Action in the list of 
    // actions of an actuator so it can be executed by it
    std::string name_;
    
    // Action FNs can be out of {OFF, ON, TOGGLE, RUN}.
    // These are the actions that can be executet by each device
    ActionFn action_fn_;  
  };
  
};