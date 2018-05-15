/*
* 2018 Rico Schulz
*
*
*/

// this software continous reads the value of the shutdown button
// shutting down the whole system when pressed for 2.5 seconds
// this programm should be started after booting thus should be put in the /etc/rc.local file
#include <iostream>
#include <wiringPi.h>
#include "HConfig.h"


using namespace std;

int main(void) {
	wiringPiSetup();

	HConfig config;
	config.HandleSetup();
	int shtdwn_pin;
	shtdwn_pin = config.getBTN_SHTDWN_PIN();

	pinMode(shtdwn_pin, INPUT);

	bool value = false;

	while(true) {
		value = digitalRead(shtdwn_pin);
		if(value) {
			delay(2500);
			value = digitalRead(shtdwn_pin);
			if(value) {
				cout << "shutting down now" << endl;
				system("sudo poweroff");
			}
		}
	}
	return 0;
}
