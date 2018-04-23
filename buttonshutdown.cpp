#include <iostream>
#include <wiringPi.h>
#include "HConfig.h"


using namespace std;

int main(void)
{
	wiringPiSetup();

	HConfig config;
	config.HandleSetup();
	int shtdwn_pin;
	shtdwn_pin = config.getBTN_SHTDWN_PIN();

	pinMode(shtdwn_pin, INPUT);

	bool value=false;

	while(true)
	{
		value = digitalRead(shtdwn_pin);
		if(value)
		{
			delay(2500);
			value = digitalRead(shtdwn_pin);
			if(value)
				system("sudo poweroff");
				
		}
	}
	return 0;
}
