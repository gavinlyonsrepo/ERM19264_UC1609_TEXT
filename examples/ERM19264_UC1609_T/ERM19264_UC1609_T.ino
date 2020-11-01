
// Example file name : ERM19264_UC1609_T.ino
// Description:
// Test file for ERM19264_UC1609_T library,  
// in this mode the LCD can be used as a relatively light weight character LCD
// 8 rows and 216 characters, 7by8 ,font size (192/7 * 64/8 ), 
// you can also write bitmaps directly to screen in this mode.
// URL: https://github.com/gavinlyonsrepo/ERM19264_UC1609_T
// *****************************
// NOTES :
// (1) GPIO is for arduino UNO.
// (2) This is for hardware SPI for software SPI see ERM19264_UC1609_T_SWSPI.ino example. 
// ******************************

#include <ERM19264_UC1609_T.h>

#define VbiasPOT 0x49 //Constrast 00 to FE , 0x49 is default. user adjust

// GPIO 5-wire SPI interface
#define CD 10 // GPIO pin number pick any you want 
#define RST 9 // GPIO pin number pick any you want
#define CS 8  // GPIO pin number pick any you want
// GPIO pin number SCK(UNO 13) , HW SPI , SCK
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI
ERM19264_UC1609_T  mylcd(CD, RST, CS); // instate object , CD, RST, CS

// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(VbiasPOT); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // clear screen
  delay(500);
}

// ************** MAIN LOOP ***********
void loop() {
  while(1)
  {
    Tests();
  }
}
// ************** END OF MAIN ***********

//Function to run a group of tests
// Test 0 clear screen 
// Test 1 String function and goto function 
// Test 2 clear page function
// Test 3 character function
// Test 4 print entire font with character function

void Tests()
{
    
    // Test 0 clear screen 
    mylcd.LCDGotoXY(0, 0);
    mylcd.LCDString("No buffer mode 216 chars"); // Write a character string
    mylcd.LCDGotoXY(0, 1);
    mylcd.LCDString("(192/7 * 64/8)= 216"); // Write a character string
    delay(5000);
    mylcd.LCDFillScreen(0x00, 0); // Clear the screen
    delay(50);
    
    // Test 1 String function and goto function  
    mylcd.LCDGotoXY(0, 0); // (Col , page ) Col 0 to 191 , page 0 to 7
    mylcd.LCDString("This is text only version"); // Write a character string
    mylcd.LCDGotoXY(0, 4);
    mylcd.LCDString("123456789012345678901234567");
    mylcd.LCDGotoXY(0, 7);
    mylcd.LCDString("This is page 7");
    delay(5000);

    // Test 2 clear page function 
    mylcd.LCDGotoXY(0, 7);
    mylcd.LCDFillPage(0x00); // Clear page
    mylcd.LCDGotoXY(0, 6);
    mylcd.LCDFillPage(0x7E); // Write pattern (0111 1110) to a page
    delay(5000);
    mylcd.LCDFillScreen(0x00, 0); // Clear the screen

    // Test 3 character function 
    mylcd.LCDGotoXY(100, 2);
    mylcd.LCDChar('H');  // write single  character
    mylcd.LCDChar('2');
    delay(5000);
    mylcd.LCDFillScreen(0x00, 0); // Clear the screen

    // Test 4 print entire font with character function
    // For characters after 'z{|}' in ASCII table user can comment in UC_FONT_MOD_TWO in font file (NOTE: this will increase program size)
    // For characters before space in ASCII table user can comment in UC_FONT_MOD_One in font file (NOTE: this will increase program size)
    mylcd.LCDGotoXY(0, 0);
    uint8_t row = 1;
    unsigned char i = 1;
    for(i=UC1609_ASCII_OFFSET; i < 126 ; i++)
    {
      if (i % 27 == 0) mylcd.LCDGotoXY(0, row++);
      mylcd.LCDChar(i); 
      delay(50);
    }
    delay(10000);
    mylcd.LCDFillScreen(0x00, 0); // Clear the screen
    
}
