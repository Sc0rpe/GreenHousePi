//
// 2018 Rico Schulz
//
#include "operator.h"

void Operator::Run() {
  // TODO
  
  // Read all Sensor values
  vector<Device*> sensors = GetDevicesByType(DeviceType::SENSOR);
  for (auto it : sensors) {
    it->Run();
  }
  
  // Check Constraints
  
  // Do actions to comply with the constraints
  
  // Read Messages from shared memory 
  
  // Do actions from messages
}

std::vector<Device*> GetDevicesByType(DeviceType dtype) {
  std::vector<Device*> d = new std::vector<Device*>();
  for (auto it = devices.begin() ; it != devices.end() ; ++it) {
    if (*it.get_type() == dtype) {
        d.push_back(it);
        break;
    }
  }
  return d;
}

ghpi::Device* ghpi::Operator::GetDeviceByName(std::string dname) {
    ghpi::Device* d;
    
    for (auto it = devices.begin() ; it != devices.end() ; ++it) {
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
    if (!CheckForDuplicateDevice(device)
        devices.push_back(*device)
    else
        cout << "Device [" << device->get_name() << "] is already registered." << endl;
}

bool CheckForDuplicateDevice(Device* device) {	
	for (std::vector<Device>::iterator it = devices.begin() ; it != devices.end() ; ++it) {
        if (*it == device) {
            d = it;
            return true;
        }
    }
	return false;
}