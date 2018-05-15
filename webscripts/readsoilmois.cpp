/*
* 2018 Rico Schulz
*/
#include <iostream>
#include <wiringPi.h>
#include "mcp3008.h"
#include "HConfig.h"

#define MEASURES 6

using namespace std;

// This program reads the soil moisture from the sensor and prints it to stdout
int main() {
    wiringPiSetup();
    HConfig config;
    config.HandleSetup();
    float soilmois = 0;
    
    pinMode(config.getSOIL_RELAY_PIN(), OUTPUT);
    digitalWrite(config.getSOIL_RELAY_PIN(), 0);
    for (int i=0; i < MEASURES; ++i) {
        soilmois += ReadAnalogData(config.getMCP_CHANNEL_SOIL(), config.getMCP_CLK_PIN(), config.getMCP_MOSI_PIN(), config.getMCP_MISO_PIN(), config.getMCP_CS_PIN());
    }
    digitalWrite(config.getSOIL_RELAY_PIN(), 1);
    soilmois /= MEASURES;
    soilmois /= 1024;
    soilmois *= 100;
    cout << soilmois << "%" << endl;    

return 0;
}
