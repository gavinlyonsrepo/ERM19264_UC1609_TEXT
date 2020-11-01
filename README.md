![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/master/extras/image/colour.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Installation](#installation)
  * [Hardware](#hardware)
  * [Features](#features)
  * [Files](#files)


Overview
--------------------
* Name : ERM19264_UC1609_T
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller) for the Arduino eco-system
* Description : 

1. Arduino library.      
2. Inverse, rotate and contrast control. 
3. ASCII text strings and character text display.
4. Sleep mode.
5. light weight text only
6. custom bitmaps supported.
7. Tested with software and hardware SPI,
* Author: Gavin Lyons
* Arduino IDE: 1.8.10

* This is a truncated light weight text only version of the main library [here](https://github.com/gavinlyonsrepo/ERM19264_UC1609) ,which includes full graphic support 

Output
---------------------------------

Output Screenshot showing full font (note the font file needs a small mod to display the first 30 characters.   see below

![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609_T/blob/master/extras/image/output.jpg)

Installation
------------------------------

The library is not included in the official Arduino library manager.
Please install manually by downloading and placing in your arduino library folder.

[Installing Additional Arduino Libraries guide](https://www.arduino.cc/en/Guide/Libraries)

Hardware
----------------------------

9 pins , Vcc and GND, anode and cathode for the backlight LED and a 5-wire SPI interface.
This is a Diagram from the manufacturer showing hardware setup connected to an MCU. The example files are setup for an UNO.  The backlight control is left up to user.
If  using Hardware SPI two of  pins will be tied to the SPI CLK and MOSI lines if using software SPI you should be able use any GPIO you want for all five pins.
Datasheets are in the extras folder. 

There are 3 different colours in range, Parts used purchased from [ebay](https://www.ebay.ie/itm/2-inch-White-192x64-Graphic-LCD-Display-Module-UC1609-SPI-for-Arduino/293617684779?hash=item445cfa512b:g:10MAAOSwYV9e6xsi)
 
1. ERM19264SBS-5 V3 LCD Display UC1609C controller ,  white on blue
2. ERM19264FS-5 V3 LCD Display  UC1609C controller , black on white
3. ERM19264DNS-5 V3 LCD Display  UC1609C controller white on black

The UC1609 controller chip is a 3.3 device but the ERM LCD module has a "662k" 3.3V regulator at back.
So the ERM LCD module will  run at 5V as well if this is present. Note J1 must be shorted for 5 volt system. 
I always ran it at  3.3V both VCC  and Logic lines during testing. 
The Backlight should always be connected to 3.3V according to datasheets.  

![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/master/extras/image/connect.jpg)

Features
-------------------------

*SPI*

Hardware and software SPI. Two different class constructors. User can pick the relevant constructor, see examples files. Hardware SPI is much faster but Software SPI allows for more flexible GPIO.
selection and easy to port to other MCU's. When running Software SPI it may be necessary on very high frequency MCU to change the UC1609_HIGHFREQ_DELAY define, It is a microsecond delay by default it is at 0.

*text and fonts*

Text only, no buffer no graphics , relatively light weight. A "hello world" Sketch uses 2172 bytes (6%) of and uses 42 bytes (2%) of dynamic memory,
Turns LCD into simple character LCD(216 characters)


The font can be  truncated by two defines to save memory space.
To include the characters cut out by these defines  simply comment them in.

1.  UC_FONT_MOD_ONE , cuts out first 30 characters 
2.  UC_FONT_MOD_TWO cuts out everything after  127 characters

The font is a standard 5 by 7 ASCII font with two  columns  of padding added. So 7 by 8 in effect. In standard text size and No buffer mode this means 192/7 * 64/8 = 27 * 8 = 216 characters.

*bitmaps*

Bitmaps can still be written directly to screen.
Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) use vertical addressing draw mode. 

*User adjustments*

When the user calls LCDbegin() to start LCD they can specify a contrast setting form 0x00 to 0xFF.
Datasheet says 0x49 is default. (VbiasPOT)

It is also possible for user to change LCD bias ,  Temperature coefficient, frame rate and power control but this must be done by changing defines in header file. Choose lower frame rate for lower power, and choose higher frame rate to improve LCD contrast and minimize flicker.

| Parameter | Value |  Define | Register |
| ------ | ------ |  ------ | ------ |
| LCD bias |  9 | BIAS_RATIO_SET | BR 1:0 |
| Temp coefficient | -0.00%/ C |  TEMP_COMP_SET | TC 1:0  |
| Frame rate | 95 fps |  FRAMERATE_SET |  LC 4:3 |
| Power control | 1.4mA + internal V LCD |  PC_SET | PC 2:0 |
| V bias Bot(contrast) | 0x49h default|  Set by user with LCDbegin | PM 7:0 |

*Functions*

Functions: Detailed information on the functions can be found in comments in the library.h header file and a list of them in keywords.txt.

Files
-------------------

Src files

| Src Files| Desc |
| ------ | ------ |
| ERM19264_UC1609_T.h | library header file  |
| ERM19264_UC1609_T.cpp |  library header source   |
| LCD_FONT_CUSTOM.h |  font |

| Examples files ino  | Desc |
| ------ | ------ |
| _BITMAP | Shows use of bitmaps  |
| _MISC | Shows misc functions, rotate invert etc |
| _T | Shows use of text IN buffer mode |
| _SWSPI | Shows use of software SPI |
