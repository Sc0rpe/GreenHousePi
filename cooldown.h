/*
* 2018 Rico Schulz
*
*
*/
#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <cstdlib>


using namespace std;


typedef struct{
    int hour;
    int min;
    int sec;
} CDWN; // cooldown


class Cooldown {
    
    public: tm m_cldwn;
    private: int SIZE;

    private: Cooldown() {
        SIZE= sizeof(int) * 9;
    }
    

    public: Cooldown(string path, int pos) {
        SIZE= sizeof(int) * 9;

        this->m_cldwn = ReadCooldown(path, pos)->m_cldwn;
    }


    public: int SaveCooldown(string path, int pos) {
        fstream dat;
        dat.open(path.c_str(), fstream::out | fstream::in |fstream::binary);
        if(dat.is_open()) {
            dat.seekp((pos - 1) * SIZE, ios::beg);
            dat.write((char*)&m_cldwn.tm_hour, sizeof(int));
            dat.write((char*)&m_cldwn.tm_min, sizeof(int));
            dat.write((char*)&m_cldwn.tm_year, sizeof(int));
            dat.write((char*)&m_cldwn.tm_sec, sizeof(int));
            dat.write((char*)&m_cldwn.tm_mday, sizeof(int));
            dat.write((char*)&m_cldwn.tm_mon, sizeof(int));
            dat.write((char*)&m_cldwn.tm_wday, sizeof(int));
            dat.write((char*)&m_cldwn.tm_yday, sizeof(int));
            dat.write((char*)&m_cldwn.tm_isdst, sizeof(int));

        }
        else {
            cerr << "Could not open cooldown file!" << endl;
            return -1;
        }
        
        dat.close();
        return 0;
    }
    

    public: static Cooldown* ReadCooldown(string path, int pos) {
        Cooldown *temp = (Cooldown*)malloc(sizeof(Cooldown));
    
        fstream dat;
        dat.open(path.c_str(), fstream::in | fstream::binary);
        if(dat.is_open()) {
            for(int i=0; i < pos; ++i) {
                dat.read((char*)&temp->m_cldwn.tm_hour, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_min, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_year, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_sec, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_mday, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_mon, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_wday, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_yday, sizeof(int));
                dat.read((char*)&temp->m_cldwn.tm_isdst, sizeof(int));
            }
        }
        else {
            cerr << "Could not open cooldown file!" << endl;
            temp = NULL;
            return temp;
        }

        dat.close();
        return temp;
    }
    
    public: void ExpandCooldown(int hours, int mins, int secs) {

        if( hours < 0 || mins < 0 || secs < 0 ) {
            cout << "Invalid cooldown parameters" << endl;
            return;
        }

        m_cldwn.tm_sec += 60 * 60 * hours;
        m_cldwn.tm_sec += 60 * mins;
        m_cldwn.tm_sec += secs;

        mktime(&m_cldwn);
    }

    public: void ExpandCooldown(CDWN cd) {
            ExpandCooldown(cd.hour, cd.min, cd.sec);
        }
    
    public: bool isElapsed() {
            
            if(difftime(time(NULL), mktime(&m_cldwn)) > 0)
                return true;
            else
                return false;
        
            
        }
    public: void Reset() {
                    m_cldwn = *localtime(&rawtime);
            }

};
