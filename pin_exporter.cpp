/*
* 2018 Rico Schulz
*
* This software should be executed once after the startup 
* this will export the pins used by our devices. Important for the backend control features!
* Let this be called by: /etc/rc.local 
*
*/
#pragma once
#include <wiringPi.h>
#include <sys/types.h>
#include <unistd.h>
#include "HConfig.h"

// This function initializes wiringPi
void initialize() {
    if (wiringPiSetup () == -1) {
        cout << "could not set up wiringPi" << endl;
        exit(EXIT_FAILURE) ;
    }
    if (setuid(getuid()) < 0) {
        cout << "Dropping privileges failed. Run as Superuser\n" << endl;
        exit(EXIT_FAILURE);
    }
    piHiPri(99); // priority
}

int main() {
    HConfig config;
    
    // setting up wiringPi
    initialize();
    
    // set up the config
    if (config.HandleSetup())
        config.Print();
    else
        cout << "Could not find a config file - aborting now." << endl;
    
    // export the Pins for our control devices to OUTPUT
    // set the digitialwrite to the default state of devices
    pinMode(config.getFAN_PIN(), OUTPUT);
    digitalWrite(config.getFAN_PIN(), HIGH);
    
    pinMode(config.getLIGHT_PIN(),OUTPUT);
    digitalWrite(config.getLIGHT_PIN(), HIGH);
    
    pinMode(config.getPUMP_PIN(), OUTPUT);
    digitalWrite(config.getPUMP_PIN(), HIGH);
    
    pinMode(config.getSOIL_RELAY_PIN(), OUTPUT);
    digitalWrite(config.getSOIL_RELAY_PIN(), HIGH);
    // add motor here

    return 0;
}