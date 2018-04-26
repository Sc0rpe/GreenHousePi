/*

This programm is for creating a config file.

*/
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
	
	//get all available Settings as a container 
	vector<string> settings = config.getAvailableSettings();
	
	//iterate through all settings and get the value as input from the user
	for(vector<string>::iterator it = settings.begin(); it != settings.end(); ++it)
	{
		string value;
		cout<<*it<<"=";
		cin>>value;
		//fail check needed here!
		//TODO
		
		config.AssignByName(*it, value);
	}	

	//save the settings file here
	//TODO
	
	return 0;
}
