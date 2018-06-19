//
// 2018 Rico Schulz
//
#include "pin.h"

using ghpi::PinMode;
using ghpi::PinState;
using ghpi::PinLayout;

void ghpi::Pin::Init() {
  int mode = -1;
  int state = 0;
  
  switch (mode_) {
    case PinMode::output: {
      mode = OUTPUT;
      break;
    }
    case PinMode::input: {
      mode = INPUT;
      break;
    }
    case PinMode::pwmoutput: {
      mode = PWM_OUTPUT;
      break;
    }    
  }
  
  switch (state_) {
    case PinState::high: {
      state = 1;
      break;
    }
    case PinState::low: {
      state = 0;
      break;
    }
  }
  
  pinMode(number_, mode);
  digitalWrite(number_, state);
}

std::string ghpi::Pin::get_name() {
  return name_;
}

int ghpi::Pin::get_number() {
  return number_;
}

ghpi::PinMode ghpi::Pin::get_mode() {
  return mode_;
}

ghpi::PinState ghpi::Pin::get_state() {
  return state_;
}

ghpi::PinLayout ghpi::Pin::get_layout() {
  return layout_;
}

ghpi::Pin::Pin(int number, PinMode mode, PinState state) {
  name_ = "Pin";
  number_ = number;
  mode_ = mode;
  state_ = state;
  layout_ = PinLayout::wiringPi;
}

ghpi::Pin::Pin(int number, PinMode mode, PinState state, PinLayout layout) : Pin(number, mode, state) {
  layout_ = layout;
}

ghpi::Pin::~Pin() {
  // Set the pins to their default state on exiting the program!
  Init();
}