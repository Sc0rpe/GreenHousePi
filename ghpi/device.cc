//
// 2018 Rico Schulz
//
#include "device.h"


void Device::RegisterPin(Pin* pin, PinUsage pin_usage, OnState on_state) {
  pins_.push_back(pin);
  pin_usages_[pin] = pin_usage;
  pin_on_states_[pin] = on_state;
}

void Device::TurnOn() {
  std::vector<Pin*> switch_pins = GetPinsByUsage(PinUsage::SWITCH);
  for (auto &pin : switch_pins) {
    digitalWrite(pin->get_number(), (int)pin_on_states_.find(pin)->second);
  }
}

void Device::TurnOff() {
  std::vector<Pin*> switch_pins = GetPinsByUsage(PinUsage::SWITCH);
  for (auto &pin : switch_pins) {
    // To invert the on state we add 1 to the value and calculate mod 2 
    // if the state was 0 we will get 1 wich is still 1 mod 2
    // if the state was 1 we will get 2 wich will become 0 mod 2
    digitalWrite(pin->get_number(), ((int)pin_on_states_.find(pin)->second + 1) % 2);
  }
}

void Device::Toggle() {
  if (get_state() == DeviceState::ON)
    TurnOff();
  else
    TurnOn();
}

void ghpi::Device::Print() {
  std::cout << "Device{" << name_ << ","
    << DeviceTypeStrings[type_] << "," 
    << OperationModeStrings[mode_] << "," 
    << DeviceStateStrings[state_] << "}" ;
}

std::string Device::get_name() {
  return name_;
}

std::string Device::get_name() {
  return name_;
}

OperationMode Device::get_mode() {
  return mode_;
}

DeviceState Device::get_state() {
  return state_;
}

DeviceType Device::get_type() {
  return type_;
}

vector<Pin*> Device::GetPinsByState(PinState state) {
  std::vector<Pin*> p = new vector<Pin*>();
  for (auto it = pins_.begin() ; it != pins_.end() ; ++it) {
    if (it->get_state() == state)
      p.push_back(it);
  }
  return p;
}

vector<Pin*> Device::GetPinsByMode(PinMode mode) {
  std::vector<Pin*> p = new vector<Pin*>();
  for (auto it = pins_.begin() ; it != pins_.end() ; ++it) {
    if (it->get_mode() == mode)
      p.push_back(it);
  }
  return p;
}

vector<Pin*> GetPinsByUsage(PinUsage usage) {
  std::vector<Pin*> p = new vector<Pin*>();
  for (auto pin : pins_) {
    if (pin_usages_.find(pin)->second == usage) {
      p.push_back(pin);
    }
  }
  return p;
}

Device::Device() {
  state_ = DeviceState::OFF;
  mode_ = OperationMode::AUTONOMOUS;
  pins_ = new std::vector<Pin*>();
  pin_usages_ = new std::map<Pin*, PinUsage>();
  pin_on_states_ = new std::map<Pin*, OnState>();
}