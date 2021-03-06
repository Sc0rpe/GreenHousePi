/*
* 2018 Rico Schulz
*
*
*/

// This programm reads an analoge value from the mcp3008 on the channel of
// the soil moisture sensor
#pragma once
#include <iostream>
#include <wiringPi.h>
#include "HConfig.h"
#include "mcp3008.h"

using namespace std;

int main(int argn, char *argv[]) {
    wiringPiSetup();
    HConfig config;
    config.HandleSetup();

    cout << "SOIL_RELAY_PIN: " << config.getSOIL_RELAY_PIN() << endl;
    cout << "MCP_CS_PIN: " << config.getMCP_CS_PIN() << endl;
    cout << "MCP_MISO_PIN: " << config.getMCP_MISO_PIN() << endl;
    cout << "MCP_MOSI_PIN: " << config.getMCP_MOSI_PIN() << endl;
    cout << "MCP_CLK_PIN: " << config.getMCP_CLK_PIN() << endl;
    cout << "MCP_CHANNEL_SOIL: " << config.getMCP_CHANNEL_SOIL() << endl;



    // export the pins for the relay and power suply
    pinMode(config.getSOIL_RELAY_PIN(), OUTPUT);
    pinMode(9, OUTPUT);

    digitalWrite(9, LOW);
    digitalWrite(config.getSOIL_RELAY_PIN(), LOW);


    // wait some time 
    delay(2000);

    int soilmois= 0;
    for(int i=0; i< config.getSOIL_MEASURES(); ++i) {   
        // channel clk mosi miso cs
        soilmois += ReadAnalogData(config.getMCP_CHANNEL_SOIL(), config.getMCP_CLK_PIN(), config.getMCP_MOSI_PIN(), config.getMCP_MISO_PIN(), config.getMCP_CS_PIN());

    }
    soilmois = soilmois / config.getSOIL_MEASURES();
    cout << "Soilmoisture: " << soilmois << endl;

    digitalWrite(config.getSOIL_RELAY_PIN(), HIGH);
    
    return 0;
}
