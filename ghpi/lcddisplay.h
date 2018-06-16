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
  };
}


