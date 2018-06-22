//
// 2018 Rico Schulz
//
#pragma once
#include "servo.h"

using ghpi::Servo;

void Servo::Initialize() {
    // Set the motor position to initial one
    SetUpPWM();
    SetPosition(angle_);
}

void Servo::TurnOn() {
  // Set Servo Position to max angle
  SetPosition(SERVO_ON_POS);
	state_ = ghpi::DeviceState::ON;
}

void Servo::TurnOff() {
  // Set Servo Position to min angle
  SetPosition(SERVO_OFF_POS);
	state_ = ghpi::DeviceState::OFF;
}

void Servo::SetUpPWM() {
  pwmSetMode(PWM_MODE_MS);
  
  pwmSetClock(GHPI_PWM_CLOCK);
  pwmSetRange(GHPI_PWM_RANGE);
}

int Servo::get_angle() {
  return angle_;
}

int Servo::ConvertAngleToValue(int angle) {
  float value;
  value = angle * (1.f/135.f) + 1.5f; // This gives us the time in ms for the angle
  value = value * (float)((19200000.f/(GHPI_PWM_CLOCK * GHPI_PWM_RANGE))); // Convert to value
  return (int)value;
}

void Servo::SetPosition(int angle) {  
  const int stepdelay = 40;
  
  // Do not kill the servo with impossible angles
  if (angle > 135 || angle < -135) {
    return;
  }
  
  // Get PWM Pin for Servo
  int pwm_pin = GetPinsByUsage(ghpi::PinUsage::PWM).at(0)->get_number();
  
  // Adapt stepwise to the desired angle 
  if (angle > angle_) {
    for (int step = angle_; step < angle; ++step) {
      pwmWrite(pwm_pin, ConvertAngleToValue(step));
      delay(stepdelay);
      angle_ = step;
    }
  } else {
    for (int step = angle_; step > angle; --step) {
      pwmWrite(pwm_pin, ConvertAngleToValue(step));
      delay(stepdelay);
      angle_ = step;
    }
  }
}

Servo::Servo(int max_angle, int init_angle) : Actuator() {
  name_ = "Servo_" + std::to_string(get_count());
  max_angle_ = max_angle;
  angle_ = init_angle;
}

Servo::~Servo() {
  SetPosition(SERVO_OFF_POS);
	delay(200);
}