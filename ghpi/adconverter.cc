//
// 2018 Rico Schulz
//
#include "adconverter.h"

int ghpi::ADConverter::GetValueFromChannel(int channel) {
  return ReadAnalogData(channel, clk_pin_, mosi_pin_, miso_pin_, cs_pin_);
}

// Parameters channel( 0 - 7 ); clockPin, MOSIpin, MISOpin, channel select pin
int ghpi::ADConverter::ReadAnalogData(int channel, int clk_pin, int mosi_pin, int miso_pin, int cs_pin) {
	int value=0;
	
	// first set up the GPIO for communication
	pinMode(cs_pin, OUTPUT);
	pinMode(clk_pin, OUTPUT);
	pinMode(mosi_pin, OUTPUT);
	pinMode(miso_pin, INPUT);

	digitalWrite(cs_pin, HIGH);
	digitalWrite(clk_pin, LOW);
	digitalWrite(mosi_pin, LOW);

	
	// Signalize the beginning of the communication
	digitalWrite(cs_pin, LOW);
	
	int command = channel;
	command = command | 0b00011000;

	// Generate the command for the channel selection
	for(int i=0; i < 5; ++i) {
		if(command & 0x10)
			digitalWrite(mosi_pin, HIGH);
		else
			digitalWrite(mosi_pin, LOW);

		digitalWrite(clk_pin, HIGH);
		digitalWrite(clk_pin, LOW);
		
		command = command << 1;
	}	

	// Read the answer 	
	for(int i=0; i < 11; ++i) {
		digitalWrite(clk_pin, HIGH);
		digitalWrite(clk_pin, LOW);

		value = value << 1;
		if(digitalRead(miso_pin))
			value = value | 0x01;
		
	}

	// Short pause for the mcp3008
	delay(500);
	return value;
}

ghpi::ADConverter::ADConverter(int clk_pin, int mosi_pin, int miso_pin, int cs_pin,
                                int min_val, int max_val) {
  clk_pin_ = clk_pin;
  mosi_pin_ = mosi_pin;
  miso_pin_ = miso_pin;
  cs_pin_ = cs_pin;
  min_val_ = min_val;
  max_val_ = max_val;
}

ADConverter::~ADConverter() {
}