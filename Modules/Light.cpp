/*
* 2018 Rico Schulz
*
*
*/

// This program is for reading values from the photoconductive resistor
// It is only for testing purposes
#include <iostream>
#include <wiringPi.h>
#include <cstdlib>
#include "HConfig.h"
#include "mcp3008.h"

#define MEASURES 5

using namespace std;

int main(int argc, const char* argv[]) {
    wiringPiSetup();
    HConfig config;
    int measures = atoi(argv[1]);
    while(true) {
        int sum = 0;
        for(int i=0; i < measures; ++i)
            sum += ReadAnalogData(1, config.getMCP_CLK_PIN(), config.getMCP_MOSI_PIN(), config.getMCP_MISO_PIN(), config.getMCP_CS_PIN());
        sum /= measures;
        cout << sum << endl;
    }
    return 0;
}
