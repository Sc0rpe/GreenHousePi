//
// 2018 Rico Schulz
// 
#include "dht22.h"

using ghpi::dht22_dat;

std::map<std::string, float> ghpi::DHT22::get_values() {
  std::map<std::string, float> values;
  DHTData d;
  int attempts = 0;
  // Try reading values as long as we do not have succes
  // but not more often than MAX_FAILS
  do {
    d = ReadDht22Dat(GetPinsByUsage(PinUsage::BI_DATA).at(0)->get_number());
    attempts++;
    delay(3000);
  } while (d.temp == -1 && d.hum == -1 && attempts < MAX_FAILS);
  
  // add data to return map
  values["TEMP"] = d.temp; 
  values["HUM"] =  d.hum;
  
  return values;
}


uint8_t ghpi::DHT22::sizecvt(const int read) {
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0) {  
    std::cerr << "Invalid data from wiringPi library." << std::endl;
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

ghpi::DHTData ghpi::DHT22::ReadDht22Dat(int pin) {
  std::cout << "Start reading DHT22 from pin " << pin << std::endl;
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
    std::cerr << "Data not good, skip." << std::endl;
    NewData.temp = -1;
    NewData.hum = -1;
    return NewData;
  }
} 

ghpi::DHT22::DHT22() : Sensor() {
  name_ = "DHT22_" + std::to_string(get_count());
  mode_ = OperationMode::AUTONOMOUS;
}

ghpi::DHT22::~DHT22() {
  
}