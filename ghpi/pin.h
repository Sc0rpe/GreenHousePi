//
// 2018 Rico Schulz
//
#pragma once
#include <string>

namespace ghpi {

    enum PinLayout {
        wiringPi = 0 ,
        hardware ,
        bcm
    };
    
    enum PinState {
        high = 0 ,
        low
    };
    
    enum PinMode {
        output = 0 ,
        input ,
        pwmoutput
    };

    class Pin {
    
    public:
        std::string name_;
        int number_;
        PinLayout layout_;
        PinState state_;
        PinMode mode_;
    };
};