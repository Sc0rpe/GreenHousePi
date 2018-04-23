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
		if(dat->temp != -1)
			break;
	//	else
	//		cout<<"corrupt data, trying again"<<endl;
		delay(3000);
	}
//	cout<<"HUM: "<<dat->hum<<" Temp: "<<dat->temp<<endl;
	return 0;

}
