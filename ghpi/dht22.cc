//
// 2018 Rico Schulz
// 
#include "dht22.h"

using ghpi::dht22_dat;

std::map<std::string, void*> DHT22::Run(void* env_var) {
  values_ = get_values();
  return values_;
}

std::map<std::string, void*> DHT22::get_values() {
  std::map<std::string, void*> values;
  DHTData d = ReadDht22Dat(GetPinsByUsage(PinUsage::BI_DATA).at(0).get_number());
  
  // add data to return map
  values["TEMP"] = (void*)d.temp; 
  values["HUM"] =  (void*)d.hum;
  
  return values;
}


static uint8_t DHT22::sizecvt(const int read) {
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0) {  
    cerr << "Invalid data from wiringPi library." << endl;
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

static ghpi::DHTData DHT22::ReadDht22Dat(int pin) {
  cout << "Start reading DHT22 from pin " << pin << endl;
  DHTData NewData;
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(10);
  digitalWrite(pin, LOW);
  delay(18);
  // then pull it up for 40 microseconds
  digitalWrite(pin, HIGH);
  delayMicroseconds(40); 
  // prepare to read the pin
  pinMode(pin, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(pin)) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255)
        break;
    }
    laststate = sizecvt(digitalRead(pin));

    if (counter == 255)
		break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      dht22_dat[j/8] <<= 1;
      if (counter > 16) //detect if signal was a 1 or 0 
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF))) {
    float t, h;
    h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
    h /= 10;
    t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
    t /= 10.0;
    if ((dht22_dat[2] & 0x80) != 0)
    t *= -1;

    NewData.temp = t;
    NewData.hum = h;
    return NewData;
  }
  else {
    cerr << "Data not good, skip." << endl;
    NewData.temp = -1;
    NewData.hum = -1;
    return NewData;
  }
} 