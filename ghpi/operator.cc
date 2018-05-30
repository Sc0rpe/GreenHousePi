//
// 2018 Rico Schulz
//
#include "operator.h"

using boost::interprocess;

void Operator::Run() {
  // TODO
  std::map<std::string, void*> values;
  // Read all Sensor values
  vector<Device*> sensors = GetDevicesByType(DeviceType::SENSOR);
  for (auto &it : sensors) {
    std::map<std::string, void*> m = it->Run();
    values.insert(m.begin(), m.end());
  }
  
  // Check Constraints
  std::vector<Action> actions;
  actions = CheckConstraints(values);
  
  // Execute actions to comply with the constraints
  for (auto &d_it: devices_) { // Check all devices for being an actuator
    if (dynamic_cast<ghpi::Actuator>(*d_it))
      for (auto &a_it: actions) { // Go through all actions to be executed
        std::vector<Action> d_actions = ((ghpi::Actuator*)d_it)->GetActionsByName(); // Retrieve actions the current device can execute
        for (auto &da_it: d_actions) {
          if (da_it->get_name() == a_it->get_name()) { // Does current action match the action of device?
            d_it->ExecuteAction(*a_it); // If they match we have the right device -> execute!
            std::cout << "["<<d_it->get_name() << "]" << " executing action: " << a_it->get_name() << std::endl;
          }
        }
          
      }
  }
  
  // Read Messages from shared memory 
  
  // Do actions from messages
}

std::vector<Action> ghpi::Operator::CheckConstraints(std::map<std::string, void*> values) {
  // List of Actions to be executed due of constraint violations
  std::vector<Action> actions;
  
  for (auto &v_it: values) {
      case EnviromentValue::TEMPERATURE: {
        
        break;
      }
      case EnviromentValue::HUMIDITY: {
        
        break;
      }
      case EnviromentValue::SOIL_MOISTURE: {
        
        break;
      }
      case EnviromentValue::LIGHT_INTENSITY: {
        
        break;
      }
      default: {
        assert(false);
      }
    } */
    for (auto &c_it: constraints_) {
      if (c_it->first.get_variable() == v_it->first) {
        if (! c_it->first.CheckForValue(v_it->second))
          actions.push_back(c_it->second);
      }
    }
  }
  return actions;
}

std::vector<Device*> GetDevicesByType(DeviceType dtype) {
  std::vector<Device*> d = new std::vector<Device*>();
  for (auto &it: devices) {
    if (it->get_type() == dtype) {
        d.push_back(it);
        break;
    }
  }
  return d;
}

ghpi::Device* ghpi::Operator::GetDeviceByName(std::string dname) {
    ghpi::Device* d;
    
    for (auto &it: devices) {
        if (it->get_name() == dname) {
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

  
Operator::Operator() {
  // Create shared Memory Segment for message queue
  shm_messages_ = shared_memory_object(create_only, "GHPI_Messages", read_write);
  shm_messages_.truncate(MSG_QUEUE_SIZE);
  region = mapped_region(shm_messages_, read_write);
}