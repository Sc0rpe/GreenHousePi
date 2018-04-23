#include <wiringPi.h>
#include "HConfig.h"

using namespace std;


int main(int argc, const char* argv[])
{

	HConfig config;
	config.HandleSetup();
	wiringPiSetup();
	string controlFile = config.getLIGHT_CONTROL_FILE_PATH();
	bool isOn = config.isDeviceOn(controlFile);

	//no argument specified
	//manualy until abortion
	if(argc < 2)
	{
		cout<<"operate manualy"<<endl;
		config.setDeviceOperation(controlFile, !isOn, -1);
	}
	else if(argc == 2) //time specified
	{
		int duration = atoi(argv[1]);
		if(duration > 0)
		{
			config.setDeviceOperation(controlFile, !isOn, duration);
		}
		else if(duration == -1)
		{
			config.setDeviceOperation(controlFile, !isOn, 0);
		}

	}
	
	//switch device status
	digitalWrite(config.getLIGHT_PIN(), isOn);

	return 0;
}
