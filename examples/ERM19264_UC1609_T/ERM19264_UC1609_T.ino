/*!
 * @file ERM19264_UC1609_T.ino
 * @brief Test file for ERM19264_UC1609_T library using hardware SPI.
 * This example demonstrates how to use the LCD as a lightweight character display.
 * The display is divided into 8 rows and supports 216 characters using a 7x8 font 
 * (calculated as 192/7 * 64/8). It also supports writing bitmaps directly to the screen.
 * @note 
 * - GPIO mapping is for Arduino UNO.
 * - This example uses hardware SPI. For software SPI, see `ERM19264_UC1609_T_SWSPI.ino`.
 * @see https://github.com/gavinlyonsrepo/ERM19264_UC1609_T
 */

#include <ERM19264_UC1609_T.h>

/// @cond

//Constrast 00 to FE , 0x49 is default. user adjust
#define VbiasPOT 0x49 
// Set AC [2:0] Program registers  for RAM address control. 0x00 to 0x07
// Address set to default 0x02 
#define AddressSet UC1609_ADDRESS_SET 

// GPIO 5-wire SPI interface
#define CS 10  // GPIO pin number pick any you want
#define CD  9 // GPIO pin number pick any you want 
#define RST 8 // GPIO pin number pick any you want
// GPIO pin number SDA(UNO 11) , HW SPI , MOSI
// GPIO pin number SCK(UNO 13) , HW SPI , SCK

ERM19264_UC1609_T  mylcd(CD, RST, CS); // instate object , CD, RST, CS

#define delay5 5000
#define delay50ms 50

// ************* SETUP ***************
void setup() {
  mylcd.LCDbegin(VbiasPOT, AddressSet); // initialize the LCD
  mylcd.LCDFillScreen(0x00, 0); // clear screen
  delay(50);
}

// ************** MAIN LOOP ***********
void loop() {
  while (1)
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
// Test 4 print ASCII font 1-127 with character function, define UC_FONT_MOD_ONE for 1-31 characters see font file 
// Test 5 print exended ASCII font 128-255, only work if UC_FONT_MOD_TWO defined see font file

void Tests()
{
  unsigned char test0_1[] = "No buffer mode 216 chars";
  unsigned char test0_2[] = "(192/7 * 64/8)= 216";

  unsigned char test1_1[] = "This is text only version";
  unsigned char test1_2[] = "123456789012345678901234567";
  unsigned char test1_3[] = "This is page 7";


  // Test 0 clear screen
  mylcd.LCDGotoXY(0, 0);
  mylcd.LCDString(test0_1); // Write a character string
  mylcd.LCDGotoXY(0, 1);
  mylcd.LCDString(test0_2); // Write a character string
  delay(delay5);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
  delay(delay50ms);

  // Test 1 String function and goto function
  mylcd.LCDGotoXY(0, 0); // (Col , page ) Col 0 to 191 , page 0 to 7
  mylcd.LCDString(test1_1); // Write a character string
  mylcd.LCDGotoXY(0, 4);
  mylcd.LCDString(test1_2);
  mylcd.LCDGotoXY(0, 7);
  mylcd.LCDString(test1_3);
  delay(delay5);

  // Test 2 clear page function
  mylcd.LCDGotoXY(0, 7);
  mylcd.LCDFillPage(0x00); // Clear page
  mylcd.LCDGotoXY(0, 6);
  mylcd.LCDFillPage(0x7E); // Write pattern (0111 1110) to a page
  delay(delay5);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen

  // Test 3 character function
  mylcd.LCDGotoXY(100, 2);
  mylcd.LCDChar('H');  // write single  character
  mylcd.LCDChar('2');
  delay(delay5);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen

  // Test 4 print ASCII font 0-127 with character function
  // For characters before space(0-0x20) in ASCII table user can comment in UC_FONT_MOD_One in font file (NOTE: this will increase program size)
  mylcd.LCDGotoXY(0, 0);
  uint8_t row = 1;
  unsigned char i = 1;
  for (i = UC1609_ASCII_OFFSET; i < 126 ; i++)
  {
    if (i % 27 == 0) mylcd.LCDGotoXY(0, row++);
    mylcd.LCDChar(i);
    delay(delay50ms);
  }
  delay(delay5);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen

  // TEST 5 print ASCII font 128-255 with character function
  // For characters after 'z{|}' in ASCII table user can comment in UC_FONT_MOD_TWO in font file
  // (NOTE: this will increase program size)
#ifdef UC_FONT_MOD_TWO
  mylcd.LCDGotoXY(0, 0);
  row = 1;
  unsigned char k = 1;
  for (k = 128; k < 255 ; k++)
  {
    if (k % 27 == 0) mylcd.LCDGotoXY(0, row++);
    mylcd.LCDChar(k);
    delay(delay50ms);
  }
  delay(delay5);
  mylcd.LCDFillScreen(0x00, 0); // Clear the screen
#endif
}


/// @endcond