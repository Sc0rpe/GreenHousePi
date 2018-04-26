/*
this is just a little programm for testing the dht22 sensor
*/
#include "pins.h"
#include "dht22.h"
#include "HConfig.h"

using namespace std;

int main(void)
{
	wiringPiSetup();	

	HConfig config;
	config.HandleSetup();

	DHTData* dat=NULL;
	
	while(true)
	{
		dat = read_dht22_dat(config.DHT_PIN);
		if(dat->temp != -1 && dat->hum != -1) 
			break; //we got our data
		else
			cout<<"corrupt data, trying again in 3 sec"<<endl;
		delay(3000);
	}
	
	cout<<"HUM: "<<dat->hum<<" Temp: "<<dat->temp<<endl;
	return 0;

}
