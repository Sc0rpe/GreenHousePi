//
// 2018 Rico Schulz
//
#pragma once
#include "servo.h"

using ghpi::Servo;

void Servo::Initialize() {
    // Set the motor position to initial one
    TurnOff();
    delay(reaction_time_);
}

void Servo::TurnOn() {
  // Set Servo Position to max angle
  SetPosition(max_angle_/2);
}

void Servo::TurnOff() {
  // Set Servo Position to min angle
  SetPosition(max_angle_/-2);
}

void Servo::SetUpPWM() {
  pwmSetMode(PWM_MODE_MS);
  
  pwmSetClock(GHPI_PWM_CLOCK);
  pwmSetRange(GHPI_PWM_RANGE);
}

int Servo::ConvertAngleToValue(int angle) {
  float value;
  value = angle * (1.f/135.f) + 1.5f; // This gives us the time in ms for the angle
  value = value * (float)((19200000.f/(GHPI_PWM_CLOCK * GHPI_PWM_RANGE))); // Convert to value
  return (int)value;
}

void Servo::SetPosition(int angle) {
  // Set up PWM Pin for controlling servo
  SetUpPWM();
  
  // Get PWM Pin for Servo
  int pwm_pin = GetPinsByUsage(ghpi::PinUsage::PWM).at(0)->get_number();
  
  // Convert angle to corresponding value
  pwmWrite(pwm_pin, ConvertAngleToValue(angle));
  
  // Wait for the motor
  delay(reaction_time_);
}

Servo::Servo(int max_angle) : Actuator() {
  name_ = "Servo_" + std::to_string(get_count());
  max_angle_ = max_angle;
}

Servo::~Servo() {
}