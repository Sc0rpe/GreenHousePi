#include <iostream>
#include <wiringPi.h>
#include "dht22.h"
#include "HConfig.h"

using namespace std;

int main()
{

	wiringPiSetup();
	HConfig config;
	config.HandleSetup();
	float hum = 0;

	for(int i=0; i < 6; ++i)
	{
		hum = ReadHumidity(config.getDHT22_PIN());
		if(hum != -1)
			break;
		delay(2000);
	}
	cout<<hum<<"%"<<endl;	
	


return 0;
}
