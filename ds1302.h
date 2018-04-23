/*
 * ds1302.h:
 *	Real Time clock
 *
 * Copyright (c) 2013 Gordon Henderson.
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

//Register defines
#define RTC_SECS	0
#define RTC_MINS	1
#define RTC_HOURS	2
#define RTC_DATE	3
#define RTC_MONTH	4
#define RTC_DAY		5
#define RTC_YEAR	6
#define RTC_WP		7
#define RTC_TC		8 //trickle charge
#define RTC_BM		31 //burst mode

static unsigned int masks [] = { 0x7F, 0x7F, 0x3F, 0x1F, 0x07, 0xFF};

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned int ds1302rtcRead       (const int reg) ;
extern void         ds1302rtcWrite      (const int reg, const unsigned int data) ;

extern unsigned int ds1302ramRead       (const int addr) ;
extern void         ds1302ramWrite      (const int addr, const unsigned int data) ;

extern void         ds1302clockRead     (int clockData [8]) ;
extern void         ds1302clockWrite    (const int clockData [8]) ;

extern void         ds1302trickleCharge (const int diodes, const int resistors) ;

extern void         ds1302setup         (const int clockPin, const int dataPin, const int csPin) ;

//support function
static int bcdToD(unsigned int byte, unsigned int mask)
{
	unsigned int b1, b2;
	byte &= mask;
	b1 = byte &0x0F;
	b2 = ((byte >> 4) & 0x0F) * 10;
	return b1 + b2;
}

#ifdef __cplusplus
}
#endif
