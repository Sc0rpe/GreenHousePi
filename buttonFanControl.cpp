#include <wiringPi.h>
#include <iostream>
#include "pins.h"
#include "HConfig.h"
#include "cooldown.h"
#include "global.h"


using namespace std;


int main()
{
	wiringPiSetup();

	HConfig config =  HConfig("/etc/Hydro/hydro.cfg");

	pinMode(config.getBTN_FANTOGGLE_PIN(), INPUT);

	while(true)
	{
		bool value=true;
		value = digitalRead(config.getBTN_FANTOGGLE_PIN());
		if(value)
		{
			delay(100);
			value = digitalRead(config.getBTN_FANTOGGLE_PIN());
			if(value)
			{
				pinMode(config.getFAN_PIN(), OUTPUT);
				digitalWrite(config.getFAN_PIN(), HIGH);
				cout<<"Turned Fan off"<<endl;
				Cooldown fan_cd = Cooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_FAN);
				fan_cd.ExpandCooldown(0, 45, 0);
				fan_cd.SaveCooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_FAN);
			}
		}
	}

	return 0;
}
