
/*!
 * @file ERM19264_UC1609_T_SWSPI.ino
 * @brief Test file for the ERM19264_UC1609_T library demonstrating use of software SPI mode.
 *  The only difference for the user is the constructor used when instantiating the object
 *  and the pin definitions.
 * @note 
 * - GPIO is configured for Arduino UNO.
 * - This example uses software SPI. For hardware SPI, see the other examples.
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
#define CD 9 // GPIO pin number pick any you want
#define RST 8 // GPIO pin number pick any you want
#define DIN  7 // GPIO pin number pick any you want
#define SCLK 6 // GPIO pin number pick any you want

ERM19264_UC1609_T  mylcd(CD, RST, CS, SCLK, DIN ); // instantiate object pick any GPIO you want


// ************* SETUP ***************
void setup()
{
  mylcd.LCDbegin(VbiasPOT, AddressSet); // initialize the LCD
  mylcd.LCDFillScreen(0x55, 0); // Splash screen
  delay(2000);
  mylcd.LCDFillScreen(0x00, 0);
}


// ************** MAIN LOOP ***********
void loop()
{
  // Call a function to display text
  DisplayText();
}

// ************** END OF MAIN ***********

void DisplayText()
{
  long startTime ;                    // start time for stop watch
  int count = 0;
  char countArray[10];
  char elapsedTimeArray[10];
  long elapsedTime ;                  // elapsed time for stop watch
  startTime = millis();
  unsigned char test[] =  "Software SPI speed test";
  unsigned char test2[] =  "Time Taken:";
  while (1)
  {
    mylcd.LCDGotoXY(10, 1);
    mylcd.LCDString(test);
    mylcd.LCDGotoXY(10, 2);
    if (count < 1000)
    {
      mylcd.LCDString((unsigned char*)itoa(count, countArray, 10));
    } else
    {
      elapsedTime =   millis() - startTime;
      mylcd.LCDString(test2);
      elapsedTime = elapsedTime / 1000L;
      mylcd.LCDString((unsigned char*)itoa(elapsedTime, elapsedTimeArray , 10));
    }
    delay(1);
    if (count == 1000)
    {
      while (1) {
        delay(1000);
      }; //test finished
    }
    count ++;
  }

}

/// @endcond