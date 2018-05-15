#pragma once
#include <wiringPi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "dht22.h"
#include "HConfig.h"
#include "soil.h"
#include "pins.h"
#include "global.h"
#include "log.h"
#include "ds1302.h"
#include "cooldown.h"
#include "mcp3008.h"

HConfig config;

// This function initializes wiringPi
void initialize() {
    if(wiringPiSetup () == -1) {
        cout << "could not set up wiringPi" << endl;
        exit(EXIT_FAILURE) ;
    }
    if(setuid(getuid()) < 0) {
        cout << "Dropping privileges failed. Run as Superuser\n" << endl;
        exit(EXIT_FAILURE);
    }
    piHiPri(99); // priority
}

float GetElapsedTime(clock_t t) {
    return (float)(clock()-t)/CLOCKS_PER_SEC;
}

bool isFanOperatedManualy() {
    return HConfig::isDeviceOperatedManualy(config.getFAN_CONTROL_FILE_PATH());
}

bool isLightOperatedManualy() {
    return HConfig::isDeviceOperatedManualy(config.getLIGHT_CONTROL_FILE_PATH());
}

void StartWatering(int pin) {
    cout << "start watering" << endl;
    if(!config.getPUMP_ACTIVE())
        return;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

void StopWatering(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
}

void TurnLightOn(int pin) {
    cout << "turning light on" << endl;
    if(!config.getLIGHT_ACTIVE() || isLightOperatedManualy())
        return;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    HConfig::setDeviceOperation(config.getLIGHT_CONTROL_FILE_PATH(), 1, 0);
}

void TurnLightOff(int pin) {
    if(isLightOperatedManualy())
        return;
    cout << "Turn Light off" << endl;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    HConfig::setDeviceOperation(config.getLIGHT_CONTROL_FILE_PATH(), 0, 0);
}

void TurnFanOn(int pin) {
    cout << "Turning Fan on" << endl;
    if(!config.getFAN_ACTIVE() || isFanOperatedManualy())
        return;
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
    HConfig::setDeviceOperation(config.getFAN_CONTROL_FILE_PATH(), 1, 0);
}

void TurnFanOff(int pin) {
    if(isFanOperatedManualy())
        return;
    cout << "Turning Fan off" << endl;
    digitalWrite(pin, HIGH);
    pinMode(pin, OUTPUT);
    HConfig::setDeviceOperation(config.getFAN_CONTROL_FILE_PATH(), 0, 0);
}

void exportRelayPins() {
	pinMode(config.PUMP_PIN, OUTPUT);
    pinMode(config.LIGHT_PIN, OUTPUT);
    pinMode(config.FAN_PIN, OUTPUT);
    pinMode(config.getSOIL_RELAY_PIN(), OUTPUT);

    digitalWrite(config.PUMP_PIN, HIGH);
    digitalWrite(config.LIGHT_PIN, HIGH);
    digitalWrite(config.FAN_PIN, HIGH);
    digitalWrite(config.getSOIL_RELAY_PIN(), LOW);
}



string GetDataString(DHTData * dht22, double soilmstr) {
    string s = "TEMP " + to_string(dht22->temp)+ " HUM " + to_string(dht22->hum) + " SOIL " + to_string(soilmstr) + "\n";
    return s;
}

bool checkForInternetConnection() {   
    // ping three times to validate connection
    if(system("ping -c 3 www.google.com"))
        return false;
    else
        return true;
}

int setLinuxClock() {
    char dateTime[20];
    char command[64];
    int clock[8];

    cout << "Setting Linux Clock from the DS1302." << endl;
    
    ds1302clockRead(clock); 
    
    sprintf(dateTime, "%02d%02d%02d%02d%02d%02d.%02d",
        bcdToD( clock[RTC_MONTH], masks[RTC_MONTH]),
        bcdToD( clock[RTC_DATE], masks[RTC_DATE]),
        bcdToD( clock[RTC_HOURS], masks[RTC_HOURS]),
        bcdToD( clock[RTC_MINS], masks[RTC_MINS]),
        20,
        bcdToD( clock[RTC_YEAR], masks[RTC_YEAR]),
        bcdToD( clock[RTC_SECS], masks[RTC_SECS]));

    sprintf(command, "/bin/date %s", dateTime);
    system(command);
    return 0;
}


void handleRTCSetup() {
    // setting up the RTC-Module
    ds1302setup(config.getCLOCK_CLOCK_PIN(), config.getCLOCK_DATA_PIN(), config.getCLOCK_CS_PIN());

    if(checkForInternetConnection()) {
        // we are connected to the internet
        
        // check if we want to use the RTC or receive the time from internet
        if(config.getUSE_RTC()) {
            if(config.getRTC_ALTHOUGH_CONNECTED()) {
                cout << "Using RTC although Internet Connection established." << endl;
                setLinuxClock();
            }
        }
    }
    else {
        // we are NOT connected to the internet
        // set the clock from the RTC-Module
        if(config.getUSE_RTC()) {
            cout << "No Internet Connection. Using the RTC-Module" << endl;
            setLinuxClock();
        }
        else
            cout << "No Internet Connection. Using RTC is disabled in the config" << endl;
    }
}

bool checkForDuplicateSession() {
    FILE *file = popen( "pidof Hydro", "r");
    char buffer[100];
        

    if( file == NULL)
    {   
        return false;
    }

    fscanf(file, "%100s", buffer);
    pclose(file);

    string output = buffer;

    return false;

    if(output.length() != 0)
        return false;
    else
        return true;
    
}

int main (int argc, char *argv[])
{
    float soilcounter = -1; 
    int lightIntensity = -1;
    DHTData* dhtdata; // temp and humidity are here
    time_t rawtime = time(NULL);

    if(checkForDuplicateSession())
    {
        cout << "Hydro already running. Exiting" << endl;
        return 0;
    }   
    // setting up wiringPi
    initialize();
    
    // set up the config
    if(config.HandleSetup())
        config.Print();
    else
        cout << "Could not find a config file - aborting now." << endl;

    // Set Up the DS1302 and handle the settings concerning the time or clock
    handleRTCSetup();

    tm * started = localtime(&rawtime); // time the skript is runned
    tm * timenow;
    bool Day = true;
    
    // variable for avoiding flickering of light relaypin
    bool turnLightOn = false;
    
    bool fan_cd_elapsed = false; // cooldown for fan elapsed ?
    bool pump_cd_elapsed = false; // cooldown for pump elapsed ?
    bool log_cd_elapsed = false; // cooldown for log elapsed ?
    bool soil_cd_elapsed =false; // cooldown for soil measure elapsed?

    Cooldown fan_cd = Cooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_FAN);
    Cooldown pump_cd = Cooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_PUMP);
    Cooldown log_cd = Cooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_LOG);
    Cooldown soil_cd = Cooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_SOIL);

    // create the local time
    rawtime = time(NULL);
    timenow = localtime(&rawtime);

    // read the cooldowns for fan and pump from fil
    
    cout << setw(8) << "FAN_CD "  << asctime(&fan_cd.m_cldwn);
    cout << setw(8) << "PUMP_CD " << asctime(&pump_cd.m_cldwn);
    cout << setw(8) << "LOG_CD " << asctime(&log_cd.m_cldwn);
    cout << setw(8) << "SOIL_CD " << asctime(&soil_cd.m_cldwn);

    // cooldowns will be reseted if they elapsed
    if(fan_cd.isElapsed())
    {
        cout << "fan cooldown elapsed" << endl;
        fan_cd_elapsed = true;
        fan_cd.m_cldwn = *started;
    }
    if(pump_cd.isElapsed())
    {
        cout << "pump cooldown elapsed" << endl;
        pump_cd_elapsed = true;
        pump_cd.m_cldwn = *started;
    }
    if(log_cd.isElapsed())
    {
        cout << "log cooldown elapsed" << endl;
        log_cd_elapsed = true;
        log_cd.m_cldwn = *started;
    }
    if(soil_cd.isElapsed())
    {
        cout << "soil cooldown elapsed" << endl;
        soil_cd_elapsed = true;
        soil_cd.m_cldwn = *started;
    }
    Log log(config.getLOG_FILE_PATH());

    // setup end########

    tm startlightat = config.getSTART_LIGHT();
    #ifdef DEBUG
        cout<<asctime(timenow)<<endl;
    #endif

    /////////////////////////////
    // Measure Light Intensity //
    /////////////////////////////
    for(int i=0; i < config.getLIGHT_MEASUREMENTS(); ++i)
    {
        lightIntensity += ReadAnalogData(config.getMCP_CHANNEL_LIGHT(), config.getMCP_CLK_PIN(), config.getMCP_MOSI_PIN(), config.getMCP_MISO_PIN(), config.getMCP_CS_PIN());
    }
    lightIntensity /= config.getLIGHT_MEASUREMENTS(); // average


    // check time to turn light on
    if(difftime(mktime(timenow), mktime(&startlightat)) > 0) 
        Day = true;
    else
        Day = false;
    
    if(Day)
        turnLightOn = true;
    
    // check if it is already night  
    if(timenow->tm_hour >= config.getSTART_LIGHT().tm_hour + config.getLIGHT_DUR())
    {
        Day = false;
        cout << "It is night!" << endl;
    }
    else
    {
        cout << "It is day!" << turnLightOn<<endl;
    }
    // turn light off due it is night
    if(!Day)
        turnLightOn = false;

    ////////////////////////////////////////
    // Measure Temperature and Humidity   //
    ////////////////////////////////////////
    for(int i = 0; i < 20 ; ++i)
    {
        dhtdata = read_dht22_dat(config.DHT_PIN);
        if(!(dhtdata->temp <= 0 || dhtdata->hum <= 0)) // get data from dht22
            break;
        delay(3000);// let the dht22 rest some time
    }
    if(dhtdata->temp <= 0 || dhtdata->hum <= 0 ) // no data from dht22
        cout << "Could not obtain data from DHT22" << endl;

    ///////////////////////////
    // Measure Soil Moisture //
    ///////////////////////////
    if(soil_cd_elapsed)
    {
        // measure the soilmoisture
        cout << "Start meassuring Soil on pin " << config.getMCP_CHANNEL_SOIL() << endl;
    
        // enable measurement in relay
        digitalWrite(config.getSOIL_RELAY_PIN(), LOW);
        soilcounter=0;
        for(int i=0; i < config.getSOIL_MEASUREMENTS(); ++i) // measure X times for better certainity
        {
            soilcounter += ReadAnalogData(config.getMCP_CHANNEL_SOIL(), config.getMCP_CLK_PIN(), config.getMCP_MOSI_PIN(), config.getMCP_MISO_PIN(), config.getMCP_CS_PIN());
        }
        digitalWrite(config.getSOIL_RELAY_PIN(), HIGH); // close relay pin to avoid electrolysis     
        soilcounter = soilcounter / config.getSOIL_MEASUREMENTS(); // calculate average
        
        cout << "Soil Value: " << soilcounter << endl;
        soilcounter = Hygrometer::getMoisturePercentage(soilcounter);
        
        soil_cd.ExpandCooldown(config.getSOIL_CD());
    }

    // Start Watering if necessary
    if(config.getPUMP_ACTIVE() && pump_cd_elapsed)
    {
        if(soilcounter > (config.getSOIL_COUNT() - config.getSOIL_VAR()) ) // TODO use percentage value 
        {
            // Do watering
            char s[20];
            sprintf(s, "sudo ./Pump.out %f", config.getWATERING_AMOUNT()); // get the watering amount from config and inserts it in the command
            system(s); // call the command
            pump_cd.ExpandCooldown(config.getPUMP_CD());
        }
    }
    
    //////////////////////////
    // Check Humidity       //
    //////////////////////////
    if(fan_cd_elapsed && dhtdata->hum > (config.getHUMIDITY() + config.getHUMIDITY_VAR()))
    {
        cout << "exceeded humidity" << endl;
        if(Day || !SILENT && !Day)
        {
            TurnFanOn(config.FAN_PIN);
        }
    }
    else if(dhtdata->hum < (config.getHUMIDITY() - config.getHUMIDITY_VAR()) || !Day)
    {
        TurnFanOff(config.FAN_PIN);
        if(fan_cd_elapsed)
            fan_cd.ExpandCooldown(config.getFAN_CD());          
    }

    //////////////////////////////
    // Check Temperature        //
    //////////////////////////////
    if(dhtdata->temp < (config.getDAY_TEMP() - config.getTEMP_VAR()) && Day )
    {
        cout << "undershot day temperature" << endl;    
        turnLightOn = true;
    }
    else if(dhtdata->temp > (config.getDAY_TEMP() + config.getTEMP_VAR()) && Day)
    {
        turnLightOn = false;
    }
    else if(!Day && dhtdata->temp <(config.getNIGHT_TEMP() - config.getTEMP_VAR()))
    {
        cout << "Temperature emergency"<<endl;
        turnLightOn = true;
    }
    else if(!Day && dhtdata->temp >(config.getNIGHT_TEMP() + config.getTEMP_VAR()))
    {
        turnLightOn = false;
    }
    
    if(turnLightOn)
        TurnLightOn(config.LIGHT_PIN);
    else
        TurnLightOff(config.LIGHT_PIN);

    //////////////////////
    //  LOGGING         //
    //////////////////////
    if(config.getLOG_ACTIVE() && log_cd_elapsed)
        log.AppendToLog(GetDataString(dhtdata, soilcounter));
    if(config.getWEBLOG_ACTIVE())
        log.WriteWebLog(config.getWEBLOG_FILE_PATH(), dhtdata->temp, dhtdata->hum, soilcounter,  lightIntensity); 

    fan_cd.SaveCooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_FAN);
    pump_cd.SaveCooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_PUMP);
    log_cd.SaveCooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_LOG);
    soil_cd.SaveCooldown(config.getCOOLDOWN_FILE_PATH(), CDWN_POS_SOIL);
    return 0;
}
