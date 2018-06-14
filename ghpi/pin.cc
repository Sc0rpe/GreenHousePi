//
// 2018 Rico Schulz
//
#include "pin.h"

using ghpi::PinMode;
using ghpi::PinState;
using ghpi::PinLayout;

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
}