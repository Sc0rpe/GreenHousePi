//
// 2018 Rico Schulz
//
#include "operator.h"

using ghpi::Operator;

void ghpi::Operator::Run() {
  // TODO
  std::map<std::string, float> values;
  // Read all Sensor values
  #ifdef DEBUG
    std::cout << "[Operator] Reading Sensors" << std::endl;
  #endif
  std::vector<Device*> sensors = GetDevicesByType(ghpi::DeviceType::SENSOR);
  for (auto it : sensors) {
    #ifdef DEBUG
      std::cout << "[Operator] Reading " << it->get_name() << std::endl;
    #endif
    std::map<std::string, float> m = it->Run(NULL);
    for (auto it: m) {
      // Check if the key already exists in the map
      if (values.find(it.first) != values.end()) {
        // Retrieve the value and calculate the median
        float val = values[it.first];
        val = (val + it.second) / 2.f;
        values[it.first] = val;
      } else {
        values.insert(it);
      }
    }
  }
  values_ = values;
  PrintValues();
  
  // Check Constraints
  #ifdef DEBUG
    std::cout << "[Operator] Checking Constraints" << std::endl;
  #endif
  std::vector<Action> actions;
  actions = CheckConstraints(values);
  
  #ifdef DEBUG
    std::cout << "[Operator] Actions to be executed " << actions.size() << std::endl;
  #endif
  
  #ifdef DEBUG
    std::cout << "[Operator] Executing Actions" << std::endl;
  #endif
  // Execute actions to comply with the constraints
  for (auto d_it: devices_) { 
    // Check all devices for being an actuator
    if (ghpi::Actuator *act = dynamic_cast<ghpi::Actuator*>(d_it))
      // Go through all actions to be executed
      for (auto const &a_it: actions) {
        std::vector<Action> d_actions = act->GetActionsByName(a_it.get_name()); // Retrieve actions the current device can execute
        for (auto const &da_it: d_actions) {
          act->ExecuteAction(a_it); // If they match we have the right device -> execute!
          std::cout << "[" << act->get_name() << "]" << " executing action: " << a_it.get_name() << std::endl;
        }     
      }
  }
  
  #ifdef DEBUG
    std::cout << "[Operator] Reading from shared memory" << std::endl;
  #endif
  // Read Messages from shared memory 
  
  #ifdef DEBUG
    std::cout << "[Operator] Executing actions from shared memory" << std::endl;
  #endif
  // Do actions from messages
  
  // Refresh display values
  if (display_) {
    std::thread showvals(&ghpi::Operator::RefreshDisplay, std::ref(values), std::ref(*display_));
    showvals.detach();
  }
}

void ghpi::Operator::RegisterConstraint(Constraint constraint, Action action) {
    if (constraints_.find(constraint) == constraints_.end()) {
      // Constraint not in the map. Add it to the map with the action.
      std::vector<Action> newaction;
      newaction.push_back(action);
      constraints_[constraint] = newaction;
      #ifdef DEBUG
        std::cout << "[Operator] added constraint " << constraint.get_name() << std::endl;
      #endif
    } else {
      // Constraint already in the map. Add the action to the vector if not already in it.
      std::vector<Action>* act = &constraints_[constraint];
      if (std::find(act->begin(), act->end(), action) == act->end()) {
        act->push_back(action);
        std::cout << "[Operator] added action " << action.get_name() << " to existing constraint " << constraint.get_name() << std::endl;
      }
    }    
}

std::vector<ghpi::Action> ghpi::Operator::CheckConstraints(std::map<std::string, float> values) {
  // List of Actions to be executed due of constraint violations
  std::vector<ghpi::Action> actions;
  // Go over the map of constraints and add each action associated with it
  // to the vector of actions to be executed if the constraint has not been met
  for (auto &c_it: constraints_) {
    for (auto &v_it: values) {
      if (c_it.first.get_variable() == v_it.first) {
        if (!c_it.first.CheckForValue(v_it.second)) {
            // Constraint has not been met, add action
            std::cout << "Constraint " << c_it.first.get_name() << " has not been met" << std::endl;
            actions.insert(actions.end(), c_it.second.begin(), c_it.second.end());
        } else {
          std::cout << "Constraint " << c_it.first.get_name() << " has been met" << std::endl;
        }
        break;
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

void ghpi::Operator::PrintDevices() {
  for (auto const it: devices_) {
    it->Print();
    std::cout << std::endl;
  }
}

void ghpi::Operator::PrintValues() {
  for (auto const &it: values_) {
    std::cout << it.first << " = " << it.second << std::endl;
  }
}

void ghpi::Operator::PrintConstraints() {
    for (auto const &it: constraints_) {
      std::cout << it.first.get_name() << std::endl;
  }
}

void Operator::RefreshDisplay(const std::map<std::string, float> &values, LCDDisplay &disp) {
  int num_line = 0;
  disp.ClrLcd();
  for (auto const &it: values) {
    disp.lcdLoc(ghpi::LCDLINES[num_line]); 
    std::string line = it.first + " = " + std::to_string(it.second);
    disp.writeLine(line.c_str());
    ++num_line;
    if (num_line > LCDDisplay::LINES) {
      delay(2000);
      disp.ClrLcd();
      num_line = 0;
    }
  }
  #ifdef DEBUG
    std::cout << "LCD Refresh finished" << std::endl;
  #endif
}

void Operator::Set_LCDDisplay(LCDDisplay* display) {
  display_ = display;
}
  
Operator::Operator() {
  // Create shared Memory Segment for message queue
  shm_messages_ = shared_memory_object(create_only, ghpi::Operator::SHM_NAME, read_write);
  shm_messages_.truncate(MSG_QUEUE_SIZE);
  region_ = mapped_region(shm_messages_, read_write);
  display_ = NULL;
}

Operator::~Operator() {
}