//
// 2018 Rico Schulz
//
#pragma once
#include "sensor.h"

// define MAXTIMINGS for ReadDht22Dat function
#define MAXTIMINGS 85

namespace ghpi {
  
  static int dht22_dat[5] = {0, 0, 0, 0, 0};
  
  typedef struct dhtdata {
    float temp;
    float hum;
  } DHTData;
  
    class DHT22: public Sensor {
      public:
      // Functions
      std::map<std::string, void*> Run(void* env_var);
      std::vector<std::string, void*> get_values();
      DHTData ReadDht22Dat(int pin);
      static uint8_t sizecvt(const int read);
      
      private:
        // Data Member
        
    };
};