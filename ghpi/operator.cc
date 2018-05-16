//
// 2018 Rico Schulz
//
#include "operator.h"

ghpi::Device* ghpi::Operator::GetDeviceByName(std::string dname) {
    ghpi::Device* d;
    
    for (std::vector<Device>::iterator it = devices.begin() ; it != devices.end() ; ++it) {
        if (*it.get_name() == dname) {
            d = it;
            break;
        }
    }
    return d;
}

bool ghpi::Operator::ToggleDevice(std::string dname) {
    ghpi::Operator::GetDeviceByName(dname)->Toggle();
}

bool ghpi::Operator::TurnOnDevice(std::string dname) {
    ghpi::Operator::GetDeviceByName(dname)->TurnOn();
}

bool ghpi::Operator::TurnOffDevice(std::string dname) {
    ghpi::Operator::GetDeviceByName(dname)->TurnOff();
}

void ghpi::Operator::RegisterDevice(ghpi::Device* device) {
    devices.push_back(*device)
}