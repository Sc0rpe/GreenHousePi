//
// 2018 Rico Schulz
//
#include "pump.h"
using ghpi::Pump;

void Pump::ExecuteAction(ghpi::Action action) {
  switch(action.get_action_fn()) {
    case ActionFn::AFN_ON: {
      float* amount = (float*)action.get_additional_data();
      if (amount != NULL)
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
      Run(NULL);
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
  name_ = "Pump_" + std::to_string(get_count());
  mode_ = OperationMode::AUTONOMOUS;
}

Pump::~Pump() {
}
