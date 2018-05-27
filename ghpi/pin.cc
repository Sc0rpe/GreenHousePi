//
// 2018 Rico Schulz
//
#include "pin.h"


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