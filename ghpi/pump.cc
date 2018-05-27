//
// 2018 Rico Schulz
//
#include "pump.h"

void ghpi::Pump::ExecuteAction(Action action) {
  switch(action.get_action_fn()) {
    case ActionFn::ON: {
      float* amount = (float*)Action.get_additional_data();
      if (amount != null)
        Water(*amount);
      break;
    }
    case ActionFn::OFF: {
      TurnOff();
      break;
    }
    case ActionFn::TOGGLE: {
      Toggle();
      break;
    }
    case ActionFn::RUN: {
      Run();
      break;
    }
    default: {
      assert(false);
    }
  }
}

void ghpi::Pump::Water(float amount) {
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

