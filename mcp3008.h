#pragma once

#include <wiringPi.h>
#include <iostream>

using namespace std;


// pin numbers from wiring pi
// Parameters channel( 0 - 7 ); clockPin, MOSIpin, MISOpin, channel select pin
int ReadAnalogData(int channel, int clkPin, int MOSIpin, int MISOpin, int csPin) {
	int value=0;
	
	// first set up the GPIO for communication
	pinMode(csPin, OUTPUT);
	pinMode(clkPin, OUTPUT);
	pinMode(MOSIpin, OUTPUT);
	pinMode(MISOpin, INPUT);

	digitalWrite(csPin, HIGH);
	digitalWrite(clkPin, LOW);
	digitalWrite(MOSIpin, LOW);

	
	// signalize the beginning of the communication
	digitalWrite(csPin, LOW);
	
	int command = channel;
	command = command | 0b00011000;

	// generate the command for the channel selection
	for(int i=0; i < 5; ++i) {
		if(command & 0x10)
			digitalWrite(MOSIpin, HIGH);
		else
			digitalWrite(MOSIpin, LOW);

		digitalWrite(clkPin, HIGH);
		digitalWrite(clkPin, LOW);
		
		command = command << 1;
	}	

	// read the answer 
	
	for(int i=0; i < 11; ++i) {
		digitalWrite(clkPin, HIGH);
		digitalWrite(clkPin, LOW);

		value = value << 1;
		if(digitalRead(MISOpin))
			value = value | 0x01;
		
	}

	// short pause for the mcp3008
	delay(500);

	return value;

}
