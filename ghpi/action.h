//
// 2018 Rico Schulz
//
#pragma  once
#include <iostream>
#include <cstring>

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
    const char* get_name() const;
    ActionFn get_action_fn() const;
    void* get_additional_data();
    void Print();
    Action();
    ~Action();
    
    // Constructor
    //  ARGS:
    //    name: Arbitrary name for the action
    //    action_fn: Action to be executed out of the ActionFn enum
    //    add_data: Additional action data. Null for the most. ml to water for pump
    Action(const char* name, ActionFn action_fn, void* add_data);
    
    bool operator==(Action const &a2) const;
    
   private:
    // Data Members
    
    // An arbitrary name for the action.
    // Name should match an Action in the list of 
    // actions of an actuator so it can be executed by it
    char name_[128];
    
    // Action FNs can be out of {OFF, ON, TOGGLE, RUN}.
    // These are the actions that can be executet by each device
    ActionFn action_fn_; 

    // Arbitrary additional data like amount
    // of water for the pump class
    void* additional_data_;
  };
  
};
