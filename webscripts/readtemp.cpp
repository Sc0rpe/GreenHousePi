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
	float temp = 0;

	for(int i=0; i < 6; ++i)
	{
		temp = ReadTemperature(config.getDHT22_PIN());
		if(temp != -1)
			break;
		delay(2000);
	}
	cout<<temp<<"°C"<<endl;	
	


return 0;
}
