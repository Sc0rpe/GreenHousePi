/*
* 2018 Rico Schulz
*
* This is a litte test programm for controlling a servo motor
* Please be carefull with this.
* Run as root - use sudo command!
*/
#pragma once
#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

// Settings for 50Hz
#define PWMCLOCK 384
#define PWMRANGE 1000

using namespace std;


// This function initializes wiringPi
void initialize() {
    if (wiringPiSetup () == -1) {
        cout << "Could not set up wiringPi. Exiting" << endl;
        exit(EXIT_FAILURE) ;
    }
    if (setuid(getuid()) < 0) {
        cout << "Dropping privileges failed. Run as Superuser" << endl;
        exit(EXIT_FAILURE);
    }
    piHiPri(99); // priority
}

int ConvertAngleToValue(int angle) {
  float value;
  float = angle * (1.f/135.f) + 1.5f; // This gives us the time in ms for the angle
  value = value * 50.f; // Convert to value for range 1000
  return (int)value;
}

int main(void) {
    // Init wiringPi
    initialize();
    bool change_direction = false;
    int angle = -135;
    
    pinMode(1, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS); 
    
    // PWM base frequency is 19,2MHz
    // pwmSetClock() sets the divisor for this clock
    // so if we want 50Hz frequency -> 50 = 19.200.000Hz / (384 * range)
    pwmSetClock(PWMCLOCK); //clock at 50Hz (20ms tick) 
    
    // Set range for specifying the pulse width
    // 1000 is range for us -> if we later write with a value of 50 we get -> 50 / 1000 = 0.005 -> 20ms * 0.005 = 0.5ms high pulse
    pwmSetRange(PWMRANGE); 
    
    while (true) {   

      if(angle < -135) {
        angle = -135;
        change_direction = true;
      }
      if(angle >= 135) {
        angle = 135;
        change_direction = false;
      }
      // first parameter is the pin number
      // second param specifies the duration of the high pulse within each pulse
      pwmWrite(1, ConvertAngleToValue(angle));  //theretically 50 (1ms) to 100 (2ms)
      delay(1000);
      
      // Change angle
      if (change_direction)
        angle = angle + 3;
      else
        angle = angle - 3;
    }
    
    return 0;
}