//
// 2018 Rico Schulz
//
#include "pump.h"
using ghpi::Pump;

void Pump::ExecuteAction(Action action) {
  switch(action.get_action_fn()) {
    case ActionFn::AFN_ON: {
      float* amount = (float*)Action.get_additional_data();
      if (amount != null)
        Water(*amount);
      break;
    }
    case ActionFn::AFN_OFF: {
      TurnOff();
      break;
    }
    case ActionFn::AFN_TOGGLE: {
      Toggle();
      break;
    }
    case ActionFn::AFN_RUN: {
      Run();
      break;
    }
    default: {
      assert(false);
    }
  }
}

void Pump::Water(float amount) {
  // Calculate time for watering
  float seconds = amount / ml_per_sec_;
  if (seconds <= 0) {
    assert(false);
  }
  
  TurnOn();
  delay(seconds * 1000);
  TurnOff();
}

void ghpi::Pump::Print() {

}

Pump::Pump() : Actuator() {
}

Pump::~Pump() : ~Actuator() {
}
