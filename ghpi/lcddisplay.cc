//
// 2018 Tobias Braeuer
//

#include "lcddisplay.h"


// Define device parameter for I2C-Display
#define I2C_ADDR   0x27 // I2C device address

// Define device constants
#define LCD_CHR  1 // Mode - Sending data
#define LCD_CMD  0 // Mode - Sending command

#define LINE1  0x80 // 1st line
#define LINE2  0xC0 // 2nd line
#define LINE3  0x94 // 3rd line
#define LINE4  0xD4 // 4th line

#define LCD_BACKLIGHT_ON   0x08  // On
#define LCD_BACKLIGHT_OFF  0x00  // Off
#define ENABLE  0b00000100 // Enable bit

const int ghpi::LCDDisplay::LINES = 4;


void ghpi::LCDDisplay::startTest(void)
{

    if (wiringPiSetup () == -1)
      exit (1);

    fd = wiringPiI2CSetup(I2C_ADDR);

    lcd_init(); // setup LCD

    while(1){
        lcdLoc(LINE1);
        writeLine("Using wiringPi");
        lcdLoc(LINE2);
        writeLine("and NANO editor.");

        delay(2000);
        ClrLcd();
        lcdLoc(LINE1);
        writeLine("I2C Programmed (TM, C, R)");
        lcdLoc(LINE2);
        writeLine("in C ... not Python.");
        delay(1000);
        lcdLoc(LINE1);
        writeLine("########################################");
        delay(1000);
        lcdLoc(LINE4);
        writeLine("**********");
        delay(1000);
        lcdLoc(LINE2);
        writeLine("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
        delay(3000);
        ClrLcd();
    }
}


// float to string
void ghpi::LCDDisplay::typeFloat(float f){
	char buffer[20];
	sprintf(buffer, "%4.2f",  f);
	writeLine(buffer);
}

// int to string
void ghpi::LCDDisplay::typeInt(int i){
    char buffer[20];
    sprintf(buffer, "%d",  i);
    writeLine(buffer);
}

// This function clears the LCD
// and resets the current location
// to the first line (0x80)
void ghpi::LCDDisplay::ClrLcd(void){
    lcd_byte(0x01, LCD_CMD);
    lcd_byte(0x02, LCD_CMD);
}

// Set the current location on LCD
void ghpi::LCDDisplay::lcdLoc(int line){
    lcd_byte(line, LCD_CMD);
}

// out char to LCD at current position
void ghpi::LCDDisplay::typeChar(char val){

    lcd_byte(val, LCD_CHR);
}

// this allows use of any size string
void ghpi::LCDDisplay::writeLine(const char *s){
    while ( *s ) lcd_byte(*(s++), LCD_CHR);
}

void ghpi::LCDDisplay::lcd_byte(int bits, int mode){
  //Send byte to data pins
  // bits = the data
  // mode = 1 for data, 0 for command
  int bits_high;
  int bits_low;
  // uses the two half byte writes to LCD
  bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT_ON ;
  bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT_ON ;

  // High bits
  wiringPiI2CReadReg8(fd, bits_high);
  lcd_toggle_enable(bits_high);

  // Low bits
  wiringPiI2CReadReg8(fd, bits_low);
  lcd_toggle_enable(bits_low);
}

void ghpi::LCDDisplay::lcd_toggle_enable(int bits)   {
    // Toggle enable pin on LCD display
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
    delayMicroseconds(500);
}

void ghpi::LCDDisplay::lcd_init(){
    fd = wiringPiI2CSetup(I2C_ADDR);
  
    // Initialise display
    lcd_byte(0x33, LCD_CMD); // Initialise
    lcd_byte(0x32, LCD_CMD); // Initialise
    lcd_byte(0x06, LCD_CMD); // Cursor move direction
    lcd_byte(0x0C, LCD_CMD); // 0x0F On, Blink Off
    lcd_byte(0x28, LCD_CMD); // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD); // Clear display
    delayMicroseconds(500);
}

ghpi::LCDDisplay::LCDDisplay() {
    lcd_init();
}

ghpi::LCDDisplay::~LCDDisplay() {
}
