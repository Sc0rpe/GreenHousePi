#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "HConfig.h"

using namespace std;

int main(int argc, const char** argv)
{
	HConfig config;
	
	vector<string> settings = config.getAvailableSettings();

	for(vector<string>::iterator it = settings.begin(); it != settings.end(); ++it)
	{
		string value;
		cout<<*it<<"=";
		cin>>value;
		config.AssignByName(*it, value);
		
	}	







	return 0;
}
