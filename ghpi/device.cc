//
// 2018 Rico Schulz
//
#include "device.h"

using namespace ghpi;

int Device::count = 0;

void Device::RegisterPin(ghpi::Pin* pin, PinUsage pin_usage, OnState on_state) {
  pins_.push_back(pin);
  pin_usages_[pin] = pin_usage;
  pin_on_states_[pin] = on_state;
  // Initialize the pin 
  pin->Init();
}

void Device::TurnOn() {
  std::vector<Pin*> switch_pins = GetPinsByUsage(PinUsage::SWITCH);
  for (auto &pin : switch_pins) {
    digitalWrite(pin->get_number(), (int)pin_on_states_.find(pin)->second);
  }
	state_ = ghpi::DeviceState::ON;
}

void Device::TurnOff() {
  std::vector<ghpi::Pin*> switch_pins = GetPinsByUsage(PinUsage::SWITCH);
  for (auto &pin : switch_pins) {
    // To invert the on state we add 1 to the value and calculate mod 2 
    // if the state was 0 we will get 1 wich is still 1 mod 2
    // if the state was 1 we will get 2 wich will become 0 mod 2
    digitalWrite(pin->get_number(), ((int)pin_on_states_.find(pin)->second + 1) % 2);
  }
	state_ = ghpi::DeviceState::OFF;
}

void Device::Toggle() {
  if (get_state() == DeviceState::ON)
    TurnOff();
  else
    TurnOn();
}

void ghpi::Device::Print() {
  std::cout << "Device{" << "ID=" << id_ << "," << name_ << ","
    << DeviceTypeStrings[static_cast<int>(type_)] << "," 
    << OperationModeStrings[static_cast<int>(mode_)] << "," 
    << DeviceStateStrings[static_cast<int>(state_)] << "}" ;
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

std::vector<ghpi::Pin*> Device::GetPinsByState(PinState state) {
  std::vector<ghpi::Pin*> p;
  for (auto &it: pins_) {
    if (it->get_state() == state)
      p.push_back(it);
  }
  return p;
}

std::vector<ghpi::Pin*> Device::GetPinsByMode(PinMode mode) {
  std::vector<ghpi::Pin*> p;
  for (auto &it: pins_) {
    if (it->get_mode() == mode)
      p.push_back(it);
  }
  return p;
}

std::vector<Pin*> Device::GetPinsByUsage(PinUsage usage) {
  std::vector<ghpi::Pin*> p;
  for (auto &pin: pins_) {
    if (pin_usages_.find(pin)->second == usage) {
      p.push_back(pin);
    }
  }
  return p;
}

bool ghpi::Device::operator==(const Device &r) const {
  if (r.name_ == name_)
    return true;
  else
    return false;
}

int ghpi::Device::get_count() {
  return count;
}

Device::Device(std::string name) {
  ++count;
  name_ = name;
  state_ = DeviceState::OFF;
  mode_ = OperationMode::AUTONOMOUS;
  id_ = get_count();
}

Device::Device() {
  ++count;
  name_ = "Device";
  state_ = DeviceState::OFF;
  mode_ = OperationMode::AUTONOMOUS;
  id_ = get_count();
}

Device::~Device() {
}