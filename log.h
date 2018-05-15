/*
* 2018 Rico Schulz
*
*
*/

#pragma once
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <string>

using namespace std;

class Log {

	private: fstream logfile;
	private: fstream weblog;
	private: string path;
	private: time_t rawtime;
	private: tm* timeinfo;

	public:	Log() {
			path = "/root/Hydro/Log.txt";
		}

	public: Log(string _path) {
			path = _path; 
		}

	public: string GetTimeAsString() {
			time(&rawtime);
			timeinfo = localtime(&rawtime);
	
			return asctime(timeinfo);
		}	

	public: tm GetTimeAsTM() {
			time(&rawtime);
			timeinfo=localtime(&rawtime);
			return *timeinfo;
		}
	public: void AppendToLog(string data) {
			logfile.open(path.c_str(), fstream::out | fstream::app);
			if(!logfile) {
				cout << "Could not open Logfile" << endl;
				return;
			}
			logfile << GetTimeAsString();
			logfile << " ";
			logfile << data << endl;
			logfile.close();
		}

	public: void PrintLogFile() {
			logfile.open(path.c_str(), fstream::in);
			if(!logfile) {
				cout << "Could not open Logifle" << endl;
				return;
			}
			char buff[256];
			while(!logfile.eof()) {
				logfile.getline(buff, 256);
				cout << buff << endl;
			}
			logfile.close();
		}
	public: string* CreateWebLogString(string temp, string hum, string soil, string light) {
			string* data = new string[4];
			data[0] = temp;
			data[1] = hum;
			data[2] = soil;
			data[3] = light;
			cout << temp << " " << hum << " " << soil << " " << light << endl;
			return data;
		}

	public: void ReadWebLog(string _path, float* temp, float* hum, int* soil, int* light) {
			weblog.open(_path.c_str(), fstream::in);
			if(weblog) {
				string input;
				getline(weblog, input);
				*temp = stof(input);
				getline(weblog, input);
				*hum = stof(input);
				getline(weblog, input);
				*soil = stoi(input);
				getline(weblog, input);
				*light = stoi(input);
				weblog.close();
			}
			else
				cerr << "Could not open Weblog File" << endl;
		}

	public: void WriteWebLog(string _path, float temp, float hum, int soil, int light) {
			
			float oldtemp, oldhum;
			int oldsoil, oldlight;
			ReadWebLog(_path, &oldtemp, &oldhum, &oldsoil, &oldlight);

			if(soil < 0)
				soil = oldsoil;
			if(light < 0)
				light = oldlight;

			weblog.open(_path.c_str(), fstream::out);
			if(weblog) {
				string output;
				stringstream ss(ios_base::app | ios_base::out);
				ss.setf(ios::fixed, ios::floatfield);
				ss.precision(2);
				ss << temp << endl << hum << endl <<soil << endl << light << endl;
				output = ss.str();
				weblog << output;
				weblog.close();

			}
			else
				cerr << "Could not open WeblogFile" << endl;
		}

	public: void WriteWebLog(string _path, string* data) {
			weblog.open(_path.c_str(), fstream::out);
			if(weblog) {
				string output;
				stringstream ss(ios_base::app | ios_base::out);
				for(int i=0; i< 4; ++i) {
					ss << data[i] << endl;
				}
				output = ss.str();
				weblog << output << endl;
				weblog.close();
			}
			else
				cerr << "Could not open Weblog file" << endl;
		}
};
