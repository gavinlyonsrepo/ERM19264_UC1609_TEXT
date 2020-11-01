
// Example file name : ERM19264_UC1609_Misc.ino
// Description:
// Test file for ERM19264_UC1609 library, showing use of various functions
//  (1) All Pixels on
//  (2) Invert screen
//  (3) Rotate screen
//  (4) Scroll Screen
//  (5) Enable and disable Screen
//
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO for other tested MCU see readme
// (2) This is for hardware SPI for software SPI see ERM19264_UC1609_SWSPI.ino example.
// ******************************

#include <ERM19264_UC1609_T.h> // Include the library

#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. USER adjust

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI

ERM19264_UC1609_T  mylcd(CD , RST, CS);

// ************* SETUP ***************
void setup()
{
  mylcd.LCDbegin(VbiasPOT);             // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0);
  delay(20);
}

// *********** MAIN LOOP ******************
void loop()
{

  // Test 1 LCD all pixels on
  mylcd.LCDGotoXY(20, 3);
  mylcd.LCDString("All Pixels on");
  delay(4000);
  mylcd.LCDFillScreen(0x00, 0);

  mylcd.LCD_allpixelsOn(1);
  delay(2000);
  mylcd.LCD_allpixelsOn(0);
  delay(2000);

  // Test 2 inverse
  mylcd.LCDGotoXY(20, 3);
  mylcd.LCDString("inverse test  ");

  mylcd.invertDisplay(0); // Normal
  delay(2000);
  mylcd.invertDisplay(1); // Inverted
  delay(4000);
  mylcd.invertDisplay(0);
  mylcd.LCDFillPage(0);

  // Test3 LCD rotate

  mylcd.LCDGotoXY(20, 3);
  mylcd.LCDString("rotate test  ");
  delay(2000);
  mylcd.LCDFillScreen(0x00, 0);

  mylcd.LCDrotate(UC1609_ROTATION_FLIP_ONE);
  mylcd.LCDString("rotated");
  delay(5000);
  mylcd.LCDrotate(UC1609_ROTATION_NORMAL);
  mylcd.LCDFillScreen(0x00, 0);
  delay(1000);

  // Test4 LCD scroll

  //
  mylcd.LCDGotoXY(20, 3);
  mylcd.LCDString("scroll test");
  for (uint8_t i = 0 ; i < 62 ; i ++)
  {
    mylcd.LCDscroll(i);

    delay(50);
  }
  mylcd.LCDscroll(0);

  //Test5 LCD enable and disable
  mylcd.LCDFillScreen(0x00, 0);
  mylcd.LCDGotoXY(20, 3);
  mylcd.LCDString("LCD Disable test");

  delay(5000);
  mylcd.LCDEnable(0);
  delay(5000);
  mylcd.LCDEnable(1);

  mylcd.LCDGotoXY(20, 3);
  mylcd.LCDFillPage(0);
  mylcd.LCDString("End");


  while (1)
  {
    delay(1); // tests over, loop here forever
  }
}

// *********** END OF MAIN ***********
