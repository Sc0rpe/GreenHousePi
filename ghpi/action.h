//
// 2018 Rico Schulz
//
#pragma  once
#include <iostream>

namespace ghpi {
  
  enum ActionFn {
    AFN_OFF = 0 ,
    AFN_ON ,
    AFN_TOGGLE ,
    AFN_RUN
  };
  static const char * ActionFnStrings[] = {"OFF", "ON", "TOGGLE", "RUN"};
  
  class Action {
   public:
    // Functions
    std::string get_name();
    std::string get_action_fn();
    void* get_additional_data();
    void Print();
    Action();
    
    // Constructor
    //  ARGS:
    //    name: Arbitrary name for the action
    //    action_fn: Action to be executed out of the ActionFn enum
    //    add_data: Additional action data. Null for the most. ml to water for pump
    Action(std::string name, ActionFn action_fn, void* add_data);
    
   private:
    // Data Members
    
    // An arbitrary name for the action.
    // Name should match an Action in the list of 
    // actions of an actuator so it can be executed by it
    std::string name_;
    
    // Action FNs can be out of {OFF, ON, TOGGLE, RUN}.
    // These are the actions that can be executet by each device
    ActionFn action_fn_; 

    // Arbitrary additional data like amount
    // of water for the pump class
    void* additional_data_;
  };
  
};
