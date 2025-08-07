![ ERM19264 ](https://github.com/gavinlyonsrepo/ERM19264_UC1609/blob/main/extras/image/color.jpg)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Output](#output)
  * [Hardware](#hardware)
  * [Features](#features)

Overview
--------------------
* Name : ERM19264_UC1609_TEXT
* Title : Library for ERM19264-5 v3 LCD  (UC1609C controller) for the Arduino eco-system
* Description : 

1. Arduino library.      
2. Inverse, rotate and contrast control. 
3. ASCII text strings and character text display.
4. Sleep mode.
5. light weight text only
6. custom bitmaps supported.
7. Tested with software and hardware SPI.

* Author: Gavin Lyons
* Arduino IDE: 1.8.10

* This is a truncated light weight text only version of the main library [ here at link ](https://github.com/gavinlyonsrepo/ERM19264_UC1609) ,which includes full graphic support  ERM19264_UC1609. Lots more info and documentation there. There is also PIC XC8 ports available 
[here at link.](https://github.com/gavinlyonsrepo/pic_16F18346_projects)

Output
---------------------------------

Output Screenshot showing ASCII font 0-127 (note the font file needs a small mod to display the first 30 characters.  see features below.) 

![op](https://github.com/gavinlyonsrepo/ERM19264_UC1609_T/blob/main/extras/image/output.jpg)


Hardware
----------------------------

See main library [ here at link ](https://github.com/gavinlyonsrepo/ERM19264_UC1609)

Features
-------------------------

*SPI*

Hardware and software SPI. Two different class constructors. User can pick the relevant constructor, see examples files. Hardware SPI is much faster but Software SPI allows for more flexible GPIO.
selection and easy to port to other MCU's. When running Software SPI it may be necessary on very high frequency MCU to change the UC1609_HIGHFREQ_DELAY define, It is a microsecond delay by default it is at 0.

*text and fonts*

Text only, no buffer or no graphics , relatively light weight. 
A "hello world" Sketch uses 2172 bytes (6%) of program memory and 42 bytes (2%) of data memory on UNO.
Turns LCD into simple character LCD(216 characters, 8 rows/pages)

The font is be  truncated by two defines to save memory space.
To include the characters cut out by these defines , simply comment them in.

1.  UC_FONT_MOD_ONE , cuts out first 30 characters 
2.  UC_FONT_MOD_TWO cuts out everything after  127 characters, [extended ASCII](https://www.extended-ascii.com/).

The font is a standard 5 by 7 ASCII font with two columns  of padding added. So 7 by 8 in effect. In standard text size and No buffer mode this means 192/7 * 64/8 = 27 * 8 = 216 characters.


*bitmaps*

Bitmaps can still be written directly to screen.
Bitmaps can be turned to data [here at link]( https://javl.github.io/image2cpp/) use vertical addressing draw mode. 
