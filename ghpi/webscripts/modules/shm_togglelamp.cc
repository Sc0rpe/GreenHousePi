//
// 2018 Rico Schulz
//
#pragma once
#include <vector>
#include <iostream>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "operator.h"
#include "action.h"

using namespace boost::interprocess;

int main() {
  boost::interprocess::shared_memory_object shm_messages_;
  boost::interprocess::mapped_region region_;
  
  shm_messages_ = shared_memory_object(open_only, ghpi::Operator::SHM_NAME, read_write);
  shm_messages_.truncate(sizeof(ghpi::Operator::MSGQueue));
  region_ = mapped_region(shm_messages_, read_write);
  
  //Get the address of the mapped region
  void * addr       = region_.get_address();
  //Construct the shared structure in memory
  ghpi::Operator::MSGQueue * data = static_cast<ghpi::Operator::MSGQueue*>(addr);
  
  ghpi::Action toggle_lamp("ToggleLamp", ghpi::ActionFn::AFN_TOGGLE, NULL, "Lamp", true);
	ghpi::Action op_manu_lamp("OperateManually", ghpi::ActionFn::AFN_OP_MANU, NULL, "Lamp", true);
  
  { // Code block for scoped_lock. Mutex will automatically unlock after block.
    // even if an exception occurs
    scoped_lock<interprocess_mutex> lock(data->mutex);
    
    // Put the action in the shared memory object
    data->Put(toggle_lamp);
		data->Put(op_manu_lamp);
  }
  
  std::cout << "Added Action " << toggle_lamp.get_name() << " to queue" << std::endl;
  
  return 0;
}