//
// 2018 Rico Schulz
//
#include "operator.h"

using ghpi::Operator;

void ghpi::Operator::Run() {
  // TODO
  std::map<std::string, float> values;
  // Read all Sensor values
  std::vector<Device*> sensors = GetDevicesByType(ghpi::DeviceType::SENSOR);
  for (auto &it : sensors) {
    std::map<std::string, float> m = it->Run(NULL);
    values.insert(m.begin(), m.end());
  }
  
  // Check Constraints
  std::vector<Action> actions;
  actions = CheckConstraints(values);
  
  // Execute actions to comply with the constraints
  for (auto d_it: devices_) { // Check all devices for being an actuator
    if (ghpi::Actuator *act = dynamic_cast<ghpi::Actuator*>(d_it))
      for (auto &a_it: actions) { // Go through all actions to be executed
        std::vector<Action> d_actions = act->GetActionsByName(a_it.get_name()); // Retrieve actions the current device can execute
        for (auto &da_it: d_actions) {
          act->ExecuteAction(a_it); // If they match we have the right device -> execute!
          std::cout << "["<<act->get_name() << "]" << " executing action: " << a_it.get_name() << std::endl;
        }     
      }
  }
  
  // Read Messages from shared memory 
  
  
  // Do actions from messages
}

std::vector<ghpi::Action> ghpi::Operator::CheckConstraints(std::map<std::string, float> values) {
  // List of Actions to be executed due of constraint violations
  std::vector<ghpi::Action> actions;
  for (auto &c_it: constraints_) {
    for (auto &v_it: values) {
      if (c_it.first.get_variable() == v_it.first) {
        if (! c_it.first.CheckForValue(v_it.second))
          actions.push_back(c_it.second);
      }
    }
  }
  return actions;
}

std::vector<ghpi::Device*> ghpi::Operator::GetDevicesByType(ghpi::DeviceType dtype) {
  std::vector<ghpi::Device*> d;
  for (auto it: devices_) {
    if (it->get_type() == dtype) {
        d.push_back(it);
        break;
    }
  }
  return d;
}

ghpi::Device* ghpi::Operator::GetDeviceByName(std::string dname) {
    ghpi::Device* d;    
    for (auto it: devices_) {
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
    if (!CheckForDuplicateDevice(device))
        devices_.push_back(device);
    else
        std::cout << "Device [" << device->get_name() << "] is already registered." << std::endl;
}

bool ghpi::Operator::CheckForDuplicateDevice(ghpi::Device* device) {	
	for (auto it: devices_) {
        if (it == device) {
            return true;
        }
    }
	return false;
}

std::vector<ghpi::Action> ghpi::Operator::ReadMessagesFromQueue() {
  // Read Messages from the Queue
  
}  
  
Operator::Operator() {
  // Create shared Memory Segment for message queue
  shm_messages_ = shared_memory_object(create_only, ghpi::Operator::SHM_NAME, read_write);
  shm_messages_.truncate(MSG_QUEUE_SIZE);
  region_ = mapped_region(shm_messages_, read_write);
}

Operator::~Operator() {
}