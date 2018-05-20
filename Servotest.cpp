/*
* 2018 Rico Schulz
*
* This is a litte test programm for controlling a servo motor
* Please be carefull with this.
* Run as root - use sudo command!
*/
#pragma once
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#define PWMCLOCK 384
#define PWMRANGE 20000
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

// Converts a given time in microseconds into the
// specific value in the range
// 500 microseconds will return 
int getValueByTime(int time) {
  double value = 0;
  value = (time * PWMRANGE) / 20;  
  return (int)value;
}

int main(void) {
    // Init wiringPi
    initialize();
    bool change_direction = false;
    int val = 500;
    
    pinMode(1, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS); 
    
    // PWM base frequency is 19,2kHz
    // pwmSetClock() sets the divisor for this clock
    // so if we want 50Hz frequency -> 50 = 19200Hz / 384
    pwmSetClock(PWMCLOCK); //clock at 50Hz (20ms tick) 
    
    // we want to have that each pulse lasts 20ms -> 20 = 1000 / 50Hz
    pwmSetRange(PWMRANGE); //range at 1000 ticks (20ms)
    
    while (true) {   
      if (val >= 2500)
        change_direction = true;
      else if ( val <= 500)
        change_direction = false;
      
      if (change_direction) {
        // Calculate new value
        val = val - 100;
      }
      else {
        val = val + 100;
      }

      // first parameter is the pin number
      // second param specifies the duration of the high pulse within each pulse
      pwmWrite(1, 500);  //theretically 50 (1ms) to 100 (2ms)
      
      delay(1000);
    }
    
    return 0;
}