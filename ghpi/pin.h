//
// 2018 Rico Schulz
//
#pragma once
#include <string>
#include <wiringPi.h>

namespace ghpi {

    enum class PinLayout : short {
      wiringPi = 0 ,
      hardware ,
      bcm
    };
    
    enum class PinState : short {
      high = 0 ,
      low
    };
    
    enum class PinMode : short {
      output = 0 ,
      input ,
      pwmoutput
    };

    class Pin { 
     public:
      // Functions
      void Init();
      std::string get_name();
      int get_number();
      ghpi::PinMode get_mode();
      ghpi::PinState get_state();
      ghpi::PinLayout get_layout();
      Pin(int number, PinMode mode, PinState state);
      Pin(int number, PinMode mode, PinState state, PinLayout layout);
      ~Pin();
      
     private:
      // Data Member
      std::string name_;
      int number_;
      ghpi::PinLayout layout_;
      ghpi::PinState state_;
      ghpi::PinMode mode_;
    };
}