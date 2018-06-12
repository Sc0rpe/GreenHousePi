//
// 2018 Rico Schulz
//
#include <wiringPi.h>

namespace ghpi {
    // Class for the MCP3008 A/D Converter
    class ADConverter {
     public:
      // Functions
      int GetValueFromChannel(int channel);
      ADConverter(int clk_pin, int mosi_pin, int miso_pin, int cs_pin,
                    int min_val, int max_val);
      ~ADConverter();
      
     private:
      int ReadAnalogData(int channel, int clk_pin, int mosi_pin,
                          int miso_pin, int cs_pin);
      
      int clk_pin_;
      int mosi_pin_;
      int miso_pin_;
      int cs_pin_;
      
      int min_val_;
      int max_val_;
    };
}