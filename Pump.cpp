#pragma once
#include <wiringPi.h>
#include <iostream>
#include "pins.h"
#include "HConfig.h"
#include <iomanip>

using namespace std;

int main(int argn, char *argv[])
{
	if(wiringPiSetup() == -1)
	{
		cout<<"could not initialize WiringPi"<<endl;
		return 0;
	}
	if(argn < 2)
	{
		cout << "needs more arguments! stopping!"<<endl;
		return 0;
	}

	HConfig config;
	config.HandleSetup();

	const double persec = config.getPUMP_ML_PER_SEC();
	double amount; //given in milliliter
	if(argn >= 2)
		amount = stoi(argv[1]);

	if(amount <= 0)
	{
		cerr << "Negative water amount - Invalid Argument"<<endl;
		return 0;
	}

	int sec; 

	sec = (int)(amount / persec);
	cout << endl;
	cout << "Amount to water = " << amount << " ml" <<endl;
	cout << sec << " seconds needed at " << persec << " ml/s"<<endl;

	pinMode(config.PUMP_PIN, OUTPUT);
	digitalWrite(config.PUMP_PIN, LOW);


	for(int i = 1 ; i <= sec; i++)
	{	cout<<"\r";
		delay(1000); //1 second
		cout << "Amount = " << setw(6)<< persec * (double)i << " ml";
		cout.flush();
	}

	cout << endl;
	digitalWrite(config.PUMP_PIN, HIGH);

	return 0;
}
