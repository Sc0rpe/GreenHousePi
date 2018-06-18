//
// 2018 Rico Schulz
//
#pragma once
#include "actuator.h"

#define GHPI_PWM_CLOCK 384
#define  GHPI_PWM_RANGE 1000

#define SERVO_OFF_POS 0
#define SERVO_ON_POS 135

namespace ghpi {
  
  // Class for the Kuman KY72 Servo
  class Servo : public Actuator {
   public:
    void TurnOn() override;
    void TurnOff() override;
    int get_angle();
    void Initialize();
    Servo(int max_angle, int init_angle);
    ~Servo();
    
   private:
    // Functions
    void SetUpPWM();
    void SetPosition(int angle);
    
    // Converts an angle to corresponding value for PWM.
    // Function assumes 50Hz and a PWM Range of 1000.
    // 0° is the neutral position of the motor
    // Angle is evenly spreaded around the neutral position.
    //  ARGS:
    //    angle: angle the motor should be steered to
    // RETURNS:
    //    The corresponding value for the PWM
    int ConvertAngleToValue(int angle);
    
    // Data Member
    // Angle the motor can totaly rotate
    int max_angle_;
    
    // Current angle of the motor
    int angle_;
   
  };
};