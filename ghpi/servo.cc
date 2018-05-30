//
// 2018 Rico Schulz
//
#pragma once
#include "servo.h"


void ghpi::Servo::Initialize() {
    // Set the motor position to initial one
    TurnOff();
    delay(reaction_time_);
}

void ghpi::Servo::TurnOn() {
  // Set Servo Position to max angle
  SetPosition(angle/2);
}

void ghpi::Servo::TurnOff() {
  // Set Servo Position to min angle
  SetPosition(angle/-2);
}

void ghpi::Servo::SetUpPWM() {
  pwmSetMode(PWM_MODE_MS);
  
  pwmSetClock(GHPI_PWM_CLOCK);
  pwmSetRange(GHPI_PWM_RANGE);
}

int ghpi::Servo::ConvertAngleToValue(int angle) {
  float value;
  value = angle * (1.f/135.f) + 1.5f; // This gives us the time in ms for the angle
  value = value * (float)((19200000.f/(GHPI_PWM_CLOCK * GHPI_PWM_RANGE)); // Convert to value
  return (int)value;
}

void ghpi::Servo::SetPosition(int angle) {
  // Set up PWM Pin for controlling servo
  SetUpPWM();
  
  // Get PWM Pin for Servo
  int pwm_pin = GetPinsByUsage(ghpi::PinUsage::PWM).at(0);
  
  // Convert angle to corresponding value
  pwmWrite(pwm_pin, ConvertAngleToValue(angle));
  
  // Wait for the motor
  delay(reaction_time_);
}