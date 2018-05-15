/*
* 2018 Rico Schulz
*
*
*/
#pragma once
#include "HConfig.h"

using namespace std;

class Hygrometer {

// this function reads the DRY and WET value set in the config and
// converts the passed value in a percentage value
public: static float getMoisturePercentage(int value) {
		HConfig config;
		config.HandleSetup();
		float perc = 0;
		float m = -1 / ((float)config.getSOIL_DRY_VALUE() - (float) config.getSOIL_WET_VALUE());
		float n = 1 - ( m * (float)config.getSOIL_WET_VALUE());
		perc = m * value + n;
		perc *= 100;

		return perc;
	}

};
