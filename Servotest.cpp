/*
* 2018 Rico Schulz
*
* This is a litte test programm for controlling a servo motor
* Please be carefull with this.
*/
#pragma once
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

using namespace std;


int main(void) {
    if (wiringPiSetup () == -1) // using wPi pin numbering
        exit (1);
    
    while (true) {
        pinMode(1, PWM_OUTPUT);
        pwmSetMode(PWM_MODE_MS); 
        
        // PWM base frequency is 19,2kHz
        // pwmSetClock() sets the divisor for this clock
        // so if we want 50Hz frequency -> 50 = 19200Hz / 384
        pwmSetClock(384); //clock at 50Hz (20us tick) 
        
        // we want to have that each pulse lasts 20ms -> 20 = 1000 / 50Hz
        pwmSetRange(1000); //range at 1000 ticks (20ms)
        
        // first parameter is the pin number
        // second param specifies the duration of the high pulse within each pulse
        pwmWrite(1, 75);  //theretically 50 (1ms) to 100 (2ms)
    }
    
    return 0;
}