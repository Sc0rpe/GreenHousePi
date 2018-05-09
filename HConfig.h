#pragma once
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h> //atoi
#include <ctime>
#include <vector>
#include "HConfigNames.h"
#include "pins.h" 
#include "cooldown.h"

using namespace std;

enum DataValue{TOLOW,OK,TOHIGH};

typedef struct{
    DataValue Temp;
    DataValue Hum;
}PlantData;


/*
handling the config file
this contains dealing with file path
and the enviroment settings of the plants
*/
class HConfig
{

private:
	double DAY_TEMP;
	double NIGHT_TEMP;
	double TEMP_VAR;

	tm START_LIGHT;
	double  LIGHT_DUR;

	double HUMIDITY;
	double HUMIDITY_VAR;

	double SOIL_COUNT;
	double SOIL_VAR;
	int SOIL_MEASUREMENTS;
	int SOIL_DRY_VALUE;
	int SOIL_WET_VALUE;

	int LIGHT_MEASUREMENTS;

	double WATERING_AMOUNT;
	double PUMP_ML_PER_SEC;

	int DAYLIGHT_THRESHOLD;	

	bool FAN_ACTIVE;
	bool PUMP_ACTIVE;
	bool LIGHT_ACTIVE;
	bool COOLDWN_ACTIVE;	
	CDWN PUMP_CD;
	CDWN FAN_CD;
	CDWN SOIL_CD;
	
	//RTC relevant values
	bool USE_RTC;
	bool RTC_ALTHOUGH_CONNECTED; //retrieve the time from RTC although connected to the internet

	//log relevant values
	bool LOG_ACTIVE;
	bool WEBLOG_ACTIVE;
	CDWN LOG_INTERVAL;
	
	//paths
	string LOG_FILE_PATH;
	string COOLDOWN_FILE_PATH;
	string PIN_MAPPING_PATH;
	string WEBLOG_FILE_PATH;
	string FAN_CONTROL_FILE_PATH;
	string LIGHT_CONTROL_FILE_PATH;
	
	
	int CLOCK_CLOCK_PIN;
	int CLOCK_DATA_PIN;
	int CLOCK_CS_PIN;

	int BTN_SHTDWN_PIN;
	int BTN_FANTOGGLE_PIN;	

	//MCP PINS AND CHANNELS
	int MCP_MOSI_PIN;
	int MCP_MISO_PIN;
	int MCP_CS_PIN;
	int MCP_CLK_PIN;
	int MCP_CHANNEL_SOIL;
	int MCP_CHANNEL_LIGHT;
	int MCP_MAX_VALUE;
	//Relay pins
	int SOIL_RELAY_PIN;

	//SERVO VARIABLES
	int SERVO_PIN;
	bool USE_SERVO;
	
public:
	int FAN_PIN;
	int LIGHT_PIN;
	int PUMP_PIN;
	int SOILMOIS_PIN;
	int DHT_PIN;


	bool initialized;	
public:
	HConfig(): DAY_TEMP(-1), NIGHT_TEMP(-1), LIGHT_DUR(-1), HUMIDITY(-1)
	{
		init();
	}

	HConfig(string path)
	{
		init();
		ReadFromAsciFile(path);
	}

	/*
	* This function takes care of the initialization of the config
	* At first it searches for a local user config. After that it will fall back to the global one.
	*/
	bool HandleSetup()
	{
		if(!this->initialized)
		{
			if(this->ReadFromAsciFile("/root/Hydro/hydro.cfg"))
			{
				cout<<"Read local user config file"<<endl;
				return true;
			}
				
		}
		if(!this->initialized)
		{
			if(this->ReadFromAsciFile("/etc/Hydro/hydro.cfg"))
			{
				cout<<"Read global config file"<<endl;
				return true;
			}
		}
		
		return false;

	}

	void init()
	{
		//setting default values
		LOG_FILE_PATH = "/etc/Hydro/Log.txt";
		WEBLOG_FILE_PATH = "/var/www/html/weblog.txt";
		COOLDOWN_FILE_PATH = "/etc/Hydro/Cooldowns.txt";
		PIN_MAPPING_PATH = "/etc/Hydro/pins.txt";
		WATERING_AMOUNT = 200; //milliliter
		PUMP_ML_PER_SEC = 0.83; //milliliter per second 
		USE_RTC = true;
		USE_SERVO = true;
		RTC_ALTHOUGH_CONNECTED = false; //retrieve the time from RTC although connected to the internet
		initialized = false;
		SOIL_MEASUREMENTS = 5;
		LIGHT_MEASUREMENTS = 5;
		SOIL_DRY_VALUE = 410;
		SOIL_WET_VALUE = 170;
		DAYLIGHT_THRESHOLD = 40;
		MCP_MAX_VALUE = 1023;
		SetDefaultPinMapping();
	}

	bool stob(string value)
	{
		if(value.find("TRUE") != string::npos)
			return true;
		else
			return false;
	}
	
	//converts a string into the value after the "=" (used for e.g. Paths)
	string stos(string s)
	{
		return s.substr(s.find("=") + 1, s.length() - s.find("=") - 1);
	}
	
	//line to double
	double ltod(string s)
	{
		double d;

		d = stod(s.substr(s.find("=") + 1,s.length() - s.find("=") - 1));

		return d;
	}

	//line to integer
	int ltoi(string s)
	{
		int i;

		i = stoi(s.substr(s.find("=") + 1,s.length() - s.find("=") - 1));

		return i;
	}

	string getValue(string s)
	{
		string val = s.substr(s.find("=") + 1,s.length() - s.find("=") - 1);
		return val;
	}
	
	string getName(string s)
	{
		string name = s.substr(0, s.find("="));
		return name;
	}

	vector<string> getAvailableSettings()
	{
		vector<string> settings;

		settings.push_back(_DAY_TEMP);
		settings.push_back(_NIGHT_TEMP);
		settings.push_back(_LIGHT_DUR);
		settings.push_back(_HUMIDITY);
		settings.push_back(_HUMIDITY_VAR);
		settings.push_back(_TEMP_VAR);
		settings.push_back(_SOIL_COUNT);
		settings.push_back(_SOIL_VAR);
		settings.push_back(_PUMP_ML_PER_SEC);
		settings.push_back(_WATERING_AMOUNT);
		settings.push_back(_START_LIGHT_HOUR);
		settings.push_back(_START_LIGHT_MIN);
		settings.push_back(_FAN_ACTIVE);
		settings.push_back(_PUMP_ACTIVE);
		settings.push_back(_LIGHT_ACTIVE);
		settings.push_back(_FAN_CD_HOUR);
		settings.push_back(_FAN_CD_MIN);
		settings.push_back(_PUMP_CD_HOUR);
		settings.push_back(_PUMP_CD_MIN);
		settings.push_back(_LOG_ACTIVE);
		settings.push_back(_LOG_FILE_PATH);
		settings.push_back(_COOLDOWN_FILE_PATH);
		settings.push_back(_PIN_MAPPING_PATH);
		settings.push_back(_USE_RTC);
		settings.push_back(_RTC_ALTHOUGH_CONNECTED);
		settings.push_back(_DAYLIGHT_THRESHOLD);
		settings.push_back(_SOIL_MEASUREMENTS);
		settings.push_back(_SOIL_WET_VALUE);
		settings.push_back(_SOIL_DRY_VALUE);
		settings.push_back(_LIGHT_MAX_VALUE);
		settings.push_back(_LIGHT_MIN_VALUE);
		settings.push_back(_LIGHT_MEASUREMENTS);
		settings.push_back(_WEBLOG_ACTIVE);
		settings.push_back(_WEBLOG_FILE_PATH);
		settings.push_back(_LIGHT_CONTROL_FILE_PATH);
		settings.push_back(_FAN_CONTROL_FILE_PATH);
		settings.push_back(_USE_SERVO);

		return settings;
		
	}

	bool AssignByName(string name, string value)
	{
		if(name == _DAY_TEMP)
			DAY_TEMP = stod(value);
		else if(name == _NIGHT_TEMP)
			NIGHT_TEMP = stod(value);
		else if(name == _LIGHT_DUR)
			LIGHT_DUR = stod(value);
		else if(name == _HUMIDITY_VAR)
			HUMIDITY_VAR = stod(value);
		else if(name == _HUMIDITY)
			HUMIDITY = stod(value);
		else if(name == _TEMP_VAR)
			TEMP_VAR = stod(value);
		else if(name == _SOIL_VAR)
			SOIL_VAR = stod(value);
		else if(name == _SOIL_COUNT)
			SOIL_COUNT = stod(value);
		else if(name == _SOIL_DRY_VALUE)
			SOIL_DRY_VALUE = stoi(value);
		else if(name == _SOIL_WET_VALUE)
			SOIL_WET_VALUE = stoi(value);
		else if(name == _FAN_ACTIVE)
			FAN_ACTIVE = stob(value);
		else if(name == _LIGHT_ACTIVE)
			LIGHT_ACTIVE = stob(value);
		else if(name == _PUMP_ACTIVE)
			PUMP_ACTIVE = stob(value);
		else if(name == _FAN_CD_HOUR)
			FAN_CD.hour = (int)stod(value);
		else if(name == _FAN_CD_MIN)
			FAN_CD.min = (int)stod(value);
		else if(name == _PUMP_CD_HOUR)
			PUMP_CD.hour = (int)stod(value);
		else if(name == _PUMP_CD_MIN)
			PUMP_CD.min = (int)stod(value);
		else if(name == _SOIL_CD_HOUR)
			SOIL_CD.hour = (int)stod(value);
		else if(name == _SOIL_CD_MIN)
			SOIL_CD.min = (int)stod(value);
		else if(name == _START_LIGHT_HOUR)
			START_LIGHT.tm_hour = (int)stod(value);
		else if(name == _START_LIGHT_MIN)
			START_LIGHT.tm_min = (int)stod(value);
		else if(name == _LOG_ACTIVE)
			LOG_ACTIVE = stob(value);
		else if(name == _LOG_INTERVAL_HOUR)
			LOG_INTERVAL.hour = (int)stod(value);
		else if(name == _LOG_INTERVAL_MIN)
			LOG_INTERVAL.min = (int)stod(value);
		else if(name == _LOG_FILE_PATH)
			LOG_FILE_PATH = value;
		else if(name == _COOLDOWN_FILE_PATH)
			COOLDOWN_FILE_PATH = value;
		else if(name == _PIN_MAPPING_PATH)
			PIN_MAPPING_PATH = value;
		else if(name == _WATERING_AMOUNT)
			WATERING_AMOUNT = stod(value);
		else if(name == _PUMP_ML_PER_SEC)
			PUMP_ML_PER_SEC = stod(value);
		else if(name == _USE_RTC)
			USE_RTC = stob(value);
		else if(name == _RTC_ALTHOUGH_CONNECTED)
			RTC_ALTHOUGH_CONNECTED = stob(value);
		else if(name == _SOIL_MEASUREMENTS)
			SOIL_MEASUREMENTS = stoi(value);
		else if(name == _LIGHT_MEASUREMENTS)
			LIGHT_MEASUREMENTS = stoi(value);
		else if(name == _DAYLIGHT_THRESHOLD)
			DAYLIGHT_THRESHOLD = stoi(value);
		else if(name == _WEBLOG_FILE_PATH)
			WEBLOG_FILE_PATH = value;					
		else if(name == _WEBLOG_ACTIVE)
			WEBLOG_ACTIVE = stob(value);
		else if(name == _MCP_MAX_VALUE)
			MCP_MAX_VALUE = stoi(value);
		else if(name == _FAN_CONTROL_FILE_PATH)
			FAN_CONTROL_FILE_PATH = value;
		else if(name == _LIGHT_CONTROL_FILE_PATH)
			LIGHT_CONTROL_FILE_PATH = value;
		else
			return false;

		return true;

	}

	bool ReadFromAsciFile(string save)
	{
		fstream datei;
		try
		{
		datei.open(save.c_str(), fstream::in);
		if(datei.is_open())
		{

			time_t now;
			time(&now);
			START_LIGHT = *localtime(&now);

			time(&now);
			START_LIGHT = *localtime(&now);


			string buff;

			do
			{
				getline(datei,buff);

				if(buff == "")
					continue;
				if(buff.at(0) == '#')
					continue;
				if(!AssignByName(getName(buff),getValue(buff)))
					 cerr<<"error in config file: -> "<<buff<<endl;

			}while(!datei.eof());

			//read the pin mapping from the file given in the config
			if(PIN_MAPPING_PATH.size() > 0)
			{
				if(!ReadPinMappingFromFile(PIN_MAPPING_PATH))
					return false;
			}		
			initialized = true;
			datei.close();
			return true;
			
		}//end if	
		}//end try
		catch(exception e)
		{
			cout<<"error reading file "<< save <<endl;
			return false;
		}
		return false;

	}

	/*
	* Depracated function. Not in use anymore!
	*/
	bool ReadFromBinaryFile(string save)
	{
		fstream datei;
		try	
		{
		datei.open(save.c_str(), fstream::in | fstream::binary);
		if(datei.is_open())
		{

			time_t now;
			time(&now);
			START_LIGHT = *localtime(&now);


			char buff[64];
			datei.read((char*)&DAY_TEMP, sizeof(double));
			datei.read((char*)&NIGHT_TEMP, sizeof(double));
			datei.read((char*)&TEMP_VAR, sizeof(double));
			datei.read((char*)&LIGHT_DUR, sizeof(double));
			datei.read((char*)&HUMIDITY, sizeof(double));
			datei.read((char*)&HUMIDITY_VAR, sizeof(double));
			datei.read((char*)&SOIL_COUNT, sizeof(double));
			datei.read((char*)&SOIL_VAR, sizeof(double));
			datei.read((char*)&START_LIGHT.tm_hour, sizeof(int));
			datei.read((char*)&START_LIGHT.tm_min, sizeof(int));		
		}
		else
		{
			cout<<"could not open plant specifiaction file"<<endl;
			return false;
		}
		datei.close();
		}
		catch(exception e)
		{
			cout<<"error reading file " << save <<endl;
			return false;
		}
		initialized = true;
		return true;
	}

	bool ReadPinMappingFromFile(string path)
	{
		fstream datei;
		try
		{
		datei.open(path.c_str(), fstream::in);
		if(datei.is_open())
		{

			string buff;

			do
			{
				getline(datei, buff);
				string temp = buff.substr(0,buff.find("="));
				if(temp == "")
					continue;
				if(temp == "LIGHT_PIN")
					 LIGHT_PIN = ltoi(buff);
				else if(temp == "FAN_PIN")
					 FAN_PIN = ltoi(buff);
				else if(temp == "PUMP_PIN")
					PUMP_PIN = ltoi(buff);
				else if(temp == "SOILMOIS_PIN")
					SOILMOIS_PIN = ltoi(buff);
				else if(temp == "DHT_PIN")
					DHT_PIN = ltoi(buff);
				else if(temp == "CLOCK_CLOCK_PIN")
					CLOCK_CLOCK_PIN = ltoi(buff);
				else if(temp == "CLOCK_DATA_PIN")
					CLOCK_DATA_PIN = ltoi(buff);
				else if(temp == "CLOCK_CS_PIN")
					CLOCK_CS_PIN = ltoi(buff);
				else if(temp == "BTN_FANTOGGLE_PIN")
					BTN_FANTOGGLE_PIN = ltoi(buff);
				else if(temp == "BTN_SHTDWN_PIN")
					BTN_SHTDWN_PIN = ltoi(buff);
				else if(temp == "MCP_MOSI_PIN")
					MCP_MOSI_PIN = ltoi(buff);
				else if(temp == "MCP_MISO_PIN")
					MCP_MISO_PIN = ltoi(buff);
				else if(temp == "MCP_CLK_PIN")
					MCP_CLK_PIN = ltoi(buff);
				else if(temp == "MCP_CS_PIN")
					MCP_CS_PIN = ltoi(buff);
				else if(temp == "MCP_CHANNEL_SOIL")
					MCP_CHANNEL_SOIL = ltoi(buff);
				else if(temp == "MCP_CHANNEL_LIGHT")
					MCP_CHANNEL_LIGHT = ltoi(buff);
				else if(temp == "SOIL_RELAY_PIN")
					SOIL_RELAY_PIN = ltoi(buff);
				else if(temp == "SERVO_PIN")
					SERVO_PIN = ltoi(buff);
				else
					 cerr<<"error in pin config file" <<endl;
			}while(!datei.eof());
		}
		datei.close();
	
		}
		catch(exception e)
		{
			cout<<"error reading pin config file "<< path <<endl;
			return false;
		}
		
		return true;
	}

	void Print()
	{
		
		cout<<setw(17)<<"DAY_TEMP: "<<DAY_TEMP<<endl;
		cout<<setw(17)<<"NIGHT_TEMP: "<<NIGHT_TEMP<<endl;
		cout<<setw(17)<<"TEMP_VAR: "<<TEMP_VAR<<endl;
		cout<<setw(17)<<"LIGHT_DUR: "<<LIGHT_DUR<<endl;
		cout<<setw(17)<<"HUMIDITY: "<<HUMIDITY<<endl;
		cout<<setw(17)<<"HUMIDITY_VAR: "<<HUMIDITY_VAR<<endl;
		cout<<setw(17)<<"SOIL_COUNT: "<<SOIL_COUNT<<endl;
		cout<<setw(17)<<"SOIL_VAR: "<<SOIL_VAR<<endl;
		cout<<setw(17)<<"START_LIGHT: "<<START_LIGHT.tm_hour<<":"<<START_LIGHT.tm_min<<endl;
		cout<<setw(17)<<"WATERING_AMOUNT: "<<WATERING_AMOUNT<<endl;
		cout<<setw(17)<<"FAN_ACTIVE: "<<FAN_ACTIVE<<endl;
		cout<<setw(17)<<"PUMP_ACTIVE: "<<PUMP_ACTIVE<<endl;
		cout<<setw(17)<<"LIGHT_ACTIVE: "<<LIGHT_ACTIVE<<endl;
		cout<<setw(17)<<"PUMP_CD: "<<PUMP_CD.hour<<":"<<PUMP_CD.min<<endl;
		cout<<setw(17)<<"FAN_CD: "<<FAN_CD.hour<<":"<<FAN_CD.min<<endl;
		cout<<setw(17)<<"PUMP_PIN: "<<PUMP_PIN<<endl;
		cout<<setw(17)<<"SOILMOIS_PIN: "<<SOILMOIS_PIN<<endl;
		cout<<setw(17)<<"SERVO_PIN: "<<SERVO_PIN<<endl;
	}	
	
	/*
	* Depracated function. Not in use anymore
	*/
	bool SaveToFile(string save)
	{
		string timestring;
		fstream datei;
		try
		{
		datei.open(save.c_str(), fstream::out | fstream::binary);
		if(datei)
		{
			cout<<"Daytemp: "; cin >> DAY_TEMP; cout<<endl;
			cout<<"Nighttemp: "; cin >> NIGHT_TEMP; cout<<endl;
			cout<<"Temperature Varianz: "; cin >> TEMP_VAR; cout<<endl;
			cout<<"Lightdur: "; cin >> LIGHT_DUR; cout<<endl;
			cout<<"Humidity: "; cin >> HUMIDITY; cout<<endl;
			cout<<"Humidity Varianz: "; cin >> HUMIDITY_VAR; cout<<endl;
			cout<<"Soil: "; cin >> SOIL_COUNT; cout<<endl;
			cout<<"Soil Variant: "; cin >> SOIL_VAR; cout<<endl;
			cout<<"Start Light at (hh:mm) : "; cin >> timestring; cout<<endl;
			
			
			datei.write((char*)&DAY_TEMP, sizeof(double));
			datei.write((char*)&NIGHT_TEMP, sizeof(double));
			datei.write((char*)&TEMP_VAR, sizeof(double));
			datei.write((char*)&LIGHT_DUR, sizeof(double));
			datei.write((char*)&HUMIDITY, sizeof(double));
			datei.write((char*)&HUMIDITY_VAR, sizeof(double));
			datei.write((char*)&SOIL_COUNT, sizeof(double));
			datei.write((char*)&SOIL_VAR, sizeof(double));
			int temphour = std::stoi(timestring.substr(0,2),nullptr,10);
			int tempmin = std::stoi(timestring.substr(3,4),nullptr,10);
			datei.write((char*)&temphour, sizeof(int));
			datei.write((char*)&tempmin, sizeof(int));
		}
		else
		{
			cout<<"Could not save file"<<endl;
			return false;
		}
		datei.close();
		}
		catch(exception e)
		{
			cout<<"error writing file"<<endl;
			return false;
		}
		return true;
	}

	void SetDefaultPinMapping()
	{
		//default Values are defined in pins.h
		LIGHT_PIN = default_LIGHT_PIN;
		FAN_PIN = default_FAN_PIN;
		PUMP_PIN = default_PUMP_PIN;
		SOILMOIS_PIN = default_SOILMOIS_PIN;
		DHT_PIN	= default_DHT_PIN;
		SERVO_PIN = default_SERVO_PIN;
		CLOCK_CLOCK_PIN = default_CLOCK_CLOCK_PIN;
		CLOCK_DATA_PIN = default_CLOCK_DATA_PIN;
		CLOCK_CS_PIN = default_CLOCK_CS_PIN;
		BTN_FANTOGGLE_PIN = default_BTN_FANTOGGLE_PIN;
		BTN_SHTDWN_PIN = default_BTN_SHTDWN_PIN;

		MCP_MISO_PIN = default_MCP_MISO_PIN;
		MCP_MOSI_PIN = default_MCP_MOSI_PIN;
		MCP_CLK_PIN = default_MCP_CLK_PIN;
		MCP_CS_PIN = default_MCP_CS_PIN;
		MCP_CHANNEL_SOIL = default_MCP_CHANNEL_SOIL;
		MCP_CHANNEL_LIGHT = default_MCP_CHANNEL_LIGHT;

		SOIL_RELAY_PIN = default_SOIL_RELAY_PIN;
	}

public: static bool isDeviceOperatedManualy(string _path)
		{
			fstream dat;
			dat.open(_path.c_str(), ios_base::in);

			if(dat)
			{
				tm temp;
				bool isManualy = false;
				string buff;
				
				getline(dat, buff);
				getline(dat, buff);
				dat.close();

				if(stol(buff) < 0)
				{
					cout<<"Undefined time operated manual"<<endl;
					return true;
				}
		
				//convert timestampt in buff into a time
				long int stamp = stol(buff);
				temp = *localtime(&stamp);
				mktime(&temp);
				time_t nowraw = time(NULL);
				tm now = *localtime(&nowraw);

				if(difftime(mktime(&now), mktime(&temp)) >= 0)
				{
					cout << "manually operation exceeded" <<endl;
					//setDeviceOperation(_path, HConfig::isDeviceOn(_path), 0);
					return false;
				}
				else
					return true;
				
				
			}
			else
				cout<<"Could not open file:"<<_path<<endl;

			return false;
		}

public: static bool isDeviceOn(string _path)
	{
		fstream dat;
		dat.open(_path.c_str(), ios_base::in);

		if(dat)
		{
			string buff;
			getline(dat, buff);
			dat.close();
		
			if(buff == "0")
				return false;
			else
				return true;
		}
		return false;

	}

public: static void setDeviceOperation(string _path, bool status, int duration)
	{
		fstream dat;
		dat.open(_path.c_str(), ios_base::out);
		if(dat)
		{
			time_t nowraw = time(NULL);
			tm now = *localtime(&nowraw);
			if(status)
				dat<<"1"<<endl;
			else
				dat<<"0"<<endl;
			if(duration >= 0)
			{
				now.tm_sec += duration * 60;
				dat << to_string(mktime(&now))<<endl;
			}
			else{
				dat << "-1"<<endl;
			}
			dat.close();
			
		}
	}


public:	//get function for member 
	
	//TEMPERATURE getter
	double getDAY_TEMP(){return DAY_TEMP;}
	double getNIGHT_TEMP(){return NIGHT_TEMP;}
	double getTEMP_VAR() { return TEMP_VAR;}

	//AMOUNT / DURATION
	double getWATERING_AMOUNT() { return WATERING_AMOUNT;}
	double getPUMP_ML_PER_SEC() { return PUMP_ML_PER_SEC; }

	tm getSTART_LIGHT(){return START_LIGHT;}
	double getLIGHT_DUR(){return LIGHT_DUR;}

	//HUMIDITY getter
	double getHUMIDITY(){return HUMIDITY;}
	double getHUMIDITY_VAR() {return HUMIDITY_VAR;}

	//SOIL getter
	double  getSOIL_COUNT(){return SOIL_COUNT;}
	double getSOIL_VAR() { return SOIL_VAR;}
	int getSOIL_MEASUREMENTS() { return SOIL_MEASUREMENTS; }
	int getSOIL_DRY_VALUE() { return SOIL_DRY_VALUE; }
	int getSOIL_WET_VALUE() { return SOIL_WET_VALUE; }
	
	//LIGHT getter
	int getDAYLIGHT_THRESHOLD() { return DAYLIGHT_THRESHOLD; }
	int getLIGHT_MEASUREMENTS() { return LIGHT_MEASUREMENTS; }

	//Device Active getter
	bool getLIGHT_ACTIVE() { return LIGHT_ACTIVE; }
	bool getFAN_ACTIVE() {return FAN_ACTIVE; }
	bool getPUMP_ACTIVE() { return PUMP_ACTIVE; }

	bool getLOG_ACTIVE() { return LOG_ACTIVE; }
	bool getWEBLOG_ACTIVE() { return WEBLOG_ACTIVE; }	

	//COOLDOWNS getter
	CDWN getFAN_CD() { return FAN_CD; }
	CDWN getPUMP_CD() { return PUMP_CD; }
	CDWN getLOG_INTERVAL() { return LOG_INTERVAL;  }
	CDWN getSOIL_CD() { return SOIL_CD; }
	
	//path getter
	string getLOG_FILE_PATH() { return LOG_FILE_PATH; }
	string getCOOLDOWN_FILE_PATH() { return COOLDOWN_FILE_PATH; }
	string getPIN_MAPPING_PATH() { return PIN_MAPPING_PATH; }
	string getWEBLOG_FILE_PATH() { return WEBLOG_FILE_PATH; }
	string getFAN_CONTROL_FILE_PATH() { return FAN_CONTROL_FILE_PATH; }
	string getLIGHT_CONTROL_FILE_PATH() { return LIGHT_CONTROL_FILE_PATH; }

	//CLOCK Getter 
	int getCLOCK_CLOCK_PIN() { return CLOCK_CLOCK_PIN; }
	int getCLOCK_DATA_PIN() { return CLOCK_DATA_PIN; }
	int getCLOCK_CS_PIN() { return CLOCK_CS_PIN; }
	bool getUSE_RTC() { return USE_RTC; }
	bool getRTC_ALTHOUGH_CONNECTED() { return RTC_ALTHOUGH_CONNECTED; }


	int getBTN_FANTOGGLE_PIN() { return BTN_FANTOGGLE_PIN; }
	int getBTN_SHTDWN_PIN() { return BTN_SHTDWN_PIN; }
	int getFAN_PIN() { return FAN_PIN; }
	int getLIGHT_PIN() { return LIGHT_PIN; }
	int getPUMP_PIN() { return PUMP_PIN; }

	int getDHT22_PIN() { return DHT_PIN; }	
	
	//MCP GETTER
	int getMCP_MISO_PIN() { return MCP_MISO_PIN; }
	int getMCP_MOSI_PIN() { return MCP_MOSI_PIN; }
	int getMCP_CS_PIN() { return MCP_CS_PIN; }
	int getMCP_CLK_PIN() { return MCP_CLK_PIN; }
	int getMCP_CHANNEL_SOIL() { return MCP_CHANNEL_SOIL; }
	int getMCP_CHANNEL_LIGHT() { return MCP_CHANNEL_LIGHT; }
	int getMCP_MAX_VALUE() { return MCP_MAX_VALUE; }
	int getSOIL_RELAY_PIN() { return SOIL_RELAY_PIN; }
	
	//SERVO GETTER
	int getSERVO_PIN() { return SERVO_PIN; }
};
