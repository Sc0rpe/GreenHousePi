//
// 2018 Rico Schulz
//
#include "operator.h"

using ghpi::Operator;

void ghpi::Operator::Run() {
  // TODO
  std::map<std::string, float> values;
  std::map<std::string, int> value_counts;
  // Read all Sensor values
  
  BOOST_LOG_TRIVIAL(info) << "[Operator] Reading Sensors";
  std::vector<Device*> sensors = GetDevicesByType(ghpi::DeviceType::SENSOR);
  for (auto it : sensors) {
    BOOST_LOG_TRIVIAL(info) << "[Operator] Reading " << it->get_name();
    std::map<std::string, float> m = it->Run(NULL);
    for (auto it: m) {
      // Check if the key already exists in the map
      if (values.find(it.first) != values.end()) {
        // Retrieve the value and add it to the sum
        float val = values[it.first];
        val = val + it.second;
        values[it.first] = val;
        ++value_counts[it.first];
        BOOST_LOG_TRIVIAL(debug) << "[Operator] Adding " << it.second << "to the sum. New Sum = " << values[it.first];
      } else {
        values.insert(it);
        value_counts.insert(std::pair<std::string, int>(it.first, 1));
        BOOST_LOG_TRIVIAL(debug) << "[Operator] Creating Environment Value " << it.second;
      }
    }
  }
  // compute the mean for each value
  for (auto it : values) {
    it.second = it.second / value_counts[it.first];
  }

  values_ = values;
  PrintValues();
  
  // Check Constraints
  BOOST_LOG_TRIVIAL(info) << "[Operator] Checking Constraints";
  std::vector<Action> actions;
  actions = CheckConstraints(values);
  BOOST_LOG_TRIVIAL(info) << "[Operator] " << actions.size() << " Actions need to be executed";

  std::vector<Device *> actuators = GetDevicesByType(DeviceType::ACTUATOR);
  for (auto a_it : actions) {
    for (auto d_it : actuators) {
      Actuator *curr_actuator = static_cast<Actuator *>(d_it);
      if (a_it.has_target() ^ a_it.get_target() != curr_actuator->get_name())
        continue;
      if (curr_actuator->CanExecute(a_it.get_name())) {
        curr_actuator->ExecuteAction(a_it);
      } else if (a_it.has_target()) {
        BOOST_LOG_TRIVIAL(error) << "Target Device " << a_it.get_target() << " cannont execute Action " << a_it.get_name();
      }
    }
  }
  
  BOOST_LOG_TRIVIAL(debug) << "[Operator] Reading from shared memory";

  // Read Messages from shared memory 
  actions = ReadMessagesFromQueue();
  BOOST_LOG_TRIVIAL(debug) << "[Operator] " << actions.size() << " to be executed from shared memory";
  BOOST_LOG_TRIVIAL(debug) << "[Operator] Executing actions from shared memory";
  // Do actions from messages
  ExecuteActions(actions);
  
  // Refresh display values
  if (display_) {
    std::thread showvals(&ghpi::Operator::RefreshDisplay, std::ref(values), std::ref(*display_));
    showvals.detach();
  }
}

void ghpi::Operator::ExecuteActions(std::vector<Action> &actions) {
	std::vector<ghpi::Action> temp;
	for (auto const &a_it: actions) {
		if (strcmp(a_it.get_target(), "\0")) {
			ghpi::Actuator *act = dynamic_cast<ghpi::Actuator*>(GetDeviceByName(a_it.get_target()));
			act->ExecuteAction(a_it);
			temp.push_back(a_it);
		}
	}

	for (auto const &it: temp) {
		int i = 0;
		for (auto const &at: actions) {
			if (actions.at(i) == it)
				break;	
			++i;
		}
		actions.erase(actions.begin() + i);
	}
	
  for (auto d_it: devices_) { 
    // Check all devices for being an actuator
    if (ghpi::Actuator *act = dynamic_cast<ghpi::Actuator*>(d_it))
      // Go through all actions to be executed
      for (auto const &a_it: actions) {
        std::vector<Action> d_actions = act->GetActionsByName(a_it.get_name()); // Retrieve actions the current device can execute
        for (auto const &da_it: d_actions) {
          act->ExecuteAction(da_it); // If they match we have the right device -> execute!
          std::cout << "[" << act->get_name() << "]" << " executing action: " << da_it.get_name() << std::endl;
        }     
      }
  }
	
}

void ghpi::Operator::RegisterConstraint(Constraint constraint, Action action) {
    if (constraints_.find(constraint) == constraints_.end()) {
      // Constraint not in the map. Add it to the map with the action.
      std::vector<Action> newaction;
      newaction.push_back(action);
      constraints_[constraint] = newaction;
      BOOST_LOG_TRIVIAL(debug) << "[Operator] added constraint " << constraint.get_name();
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
    ghpi::Device* d = nullptr;    
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
    DeviceState state = ghpi::Operator::GetDeviceByName(dname)->get_state();
    if( state == DeviceState::ON ) {
      return true;
    } else return false;
}

bool ghpi::Operator::TurnOnDevice(std::string dname) {
    ghpi::Operator::GetDeviceByName(dname)->TurnOn();
    return true;
}

bool ghpi::Operator::TurnOffDevice(std::string dname) {
    ghpi::Operator::GetDeviceByName(dname)->TurnOff();
    return false;
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
  std::vector<ghpi::Action> actions;
  
  //Get the address of the mapped region
  void * addr       = region_.get_address();
  //Construct the shared structure in memory
  MSGQueue * data = static_cast<ghpi::Operator::MSGQueue*>(addr);
  if (!data) {
    std::cout << " Error while reading shared memory" << std::endl;
    return actions;
  }
  
  { // Code block for scoped_lock. Mutex will automatically unlock after block.
    // even if an exception occurs
    scoped_lock<interprocess_mutex> lock(data->mutex);
    
    while (data->count > 0) {
      actions.push_back(data->Pop());
      std::cout << " Read action from shm" << std::endl;
    }
  }
  
  return actions;
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

void Operator::RefreshDisplay(const std::map<std::string, float> &values, ghpi::LCDDisplay &disp) {
  int num_line = 0;
  disp.ClrLcd();
  for (auto const &it: values) {
    std::string line = it.first + " = " + std::to_string(it.second);
    disp.writeLine(line.c_str(), num_line);
    ++num_line;
    if (num_line > LCDDisplay::LINES) {
      delay(2000);
      disp.ClrLcd();
      num_line = 0;
    }
  }
  BOOST_LOG_TRIVIAL(debug) << "LCD Refresh finished";
}

void Operator::Set_LCDDisplay(ghpi::LCDDisplay* display) {
  display_ = display;
}
  
Operator::Operator() {
  // Create shared Memory Segment for message queue
  BOOST_LOG_TRIVIAL(debug) << sizeof(ghpi::Operator::MSGQueue);
  shm_messages_ = shared_memory_object(create_only, ghpi::Operator::SHM_NAME, read_write);
  shm_messages_.truncate(sizeof(ghpi::Operator::MSGQueue));
  region_ = mapped_region(shm_messages_, read_write);
  display_ = NULL;
}

Operator::~Operator() {
}