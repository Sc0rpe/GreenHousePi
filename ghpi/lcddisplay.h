//
// 2018 Tobias Braeuer
//
#pragma once
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
//#include "debug.h"



namespace ghpi {
  
  enum LCDLINE {
    LINE1 = 0x80,
    LINE2 = 0xC0,
    LINE3 = 0x94,
    LINE4 = 0xD4
  };
  const int LCDLINES[] =  {LINE1, LINE2, LINE3, LINE4};
  
  class LCDDisplay {
	 public:
		void lcd_init(void);
		void lcd_byte(int bits, int mode);
		void lcd_toggle_enable(int bits);
		void typeInt(int i);
		void typeFloat(float myFloat);
		void lcdLoc(int line); //move cursor
		void ClrLcd(void); // clr LCD return home
		void writeLine(const char *s);
		void typeChar(char val);
		int fd;  // seen by all subroutines
		void startTest(void);
    LCDDisplay();
		~LCDDisplay();
    
    const static int LINES;
  };
}


