/*!
* @file ERM19264_UC1609_T.cpp
* @brief ERM19264 LCD driven by UC1609C controller source file text only version
* @author Gavin Lyons.
* @details https://github.com/gavinlyonsrepo/ERM19264_UC1609_TEXT
*/

#include "ERM19264_UC1609_T.h"


// Class Constructors , 2 off
// No buffer , Hardware + software SPI

// Hardware SPI
ERM19264_UC1609_T  :: ERM19264_UC1609_T(int8_t cd, int8_t rst, int8_t cs) 
{
	_LCD_CD = cd;
	_LCD_RST= rst;
	_LCD_CS = cs;
	_LCD_DIN = -1;   // -1 for din  specify using hardware SPI
	_LCD_SCLK = -1;  // -1 for sclk specify using hardware SPI
}

// Software SPI
ERM19264_UC1609_T :: ERM19264_UC1609_T(int8_t cd, int8_t rst, int8_t cs, int8_t sclk, int8_t din)
{
	_LCD_CD = cd;
	_LCD_RST= rst;
	_LCD_CS = cs;
	_LCD_DIN = din;  
	_LCD_SCLK = sclk;
}


/*!
	@brief begin Method initialise LCD  Sets pinmodes and SPI setup
	@param VbiasPOT contrast default = 0x49 , range 0x00 to 0xFE
	@param AddressSet AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
 */
void ERM19264_UC1609_T::LCDbegin (uint8_t VbiasPOT, uint8_t AddressSet)
{
	pinMode(_LCD_CD , OUTPUT);
	pinMode(_LCD_RST, OUTPUT);
	pinMode(_LCD_CS, OUTPUT);
	
	_VbiasPOT  = VbiasPOT;
	if (isHardwareSPI()) 
	{
	 SPI.begin();
	}else{
		 // Set software SPI specific pin outputs.
		pinMode(_LCD_DIN, OUTPUT);
		pinMode(_LCD_SCLK, OUTPUT);
	}
	LCDinit(AddressSet);
}
/*!
	@brief Called from LCDbegin carries out Power on sequence and register init
		Can be used to reset LCD to default values.
	@param AddressSet AC [2:0] registers for RAM addr ctrl. default=2 range 0-7
*/
void ERM19264_UC1609_T::LCDinit(uint8_t AddressSet)
 {
	UC1609_CD_SetHigh;
	UC1609_CS_SetHigh;
	
	delay(UC1609_POWERON_DELAY1); 
	UC1609_RST_SetLow;
	delay(UC1609_POWERON_DELAY2); 
	UC1609_RST_SetHigh;
	delay(UC1609_POWERON_DELAY3);

	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;

	send_command(UC1609_TEMP_COMP_REG, UC1609_TEMP_COMP_SET); 
	if (AddressSet > 7) AddressSet = UC1609_ADDRESS_SET;
	send_command(UC1609_ADDRESS_CONTROL, AddressSet);
	send_command(UC1609_FRAMERATE_REG, UC1609_FRAMERATE_SET);
	send_command(UC1609_BIAS_RATIO, UC1609_BIAS_RATIO_SET);  
	send_command(UC1609_POWER_CONTROL,  UC1609_PC_SET); 
	delay(UC1609_INIT_DELAY);
	
	send_command(UC1609_GN_PM, 0);
	send_command(UC1609_GN_PM, _VbiasPOT); //  changed by user
	
	send_command(UC1609_DISPLAY_ON, 0x01); // turn on display
	send_command(UC1609_LCD_CONTROL, UC1609_ROTATION_NORMAL); // rotate to normal 
	
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}


/*!
	 @brief Sends a command to the display
	 @param command Command to send
	 @param value the values to change
	 @note command and value  will be combined with OR
*/
void ERM19264_UC1609_T::send_command (uint8_t command, uint8_t value) 
{
	UC1609_CD_SetLow; 
	send_data(command | value);
	UC1609_CD_SetHigh;
}


/*!
	 @brief Turns On Display
	 @param bits  1  display on , 0 display off
*/
void ERM19264_UC1609_T::LCDEnable (uint8_t bits) 
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_DISPLAY_ON, bits);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}


/*!
	@brief Scroll the displayed image up by SL rows.
	@details The valid SL value is between 0 (for no
		scrolling) and (64).
		Setting SL outside of this range causes undefined effect on the displayed image.
	@param bits 0-64 line number y-axis
*/
void ERM19264_UC1609_T::LCDscroll (uint8_t bits) 
{
	if(isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_SCROLL, bits);
	UC1609_CS_SetHigh;
	if(isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief Rotates the display
	@details Set LC[2:1] for COM (row) mirror (MY), SEG (column) mirror (MX).
		Param1: 4 possible values 000 010 100 110 (defined)
	@note If Mx is changed the buffer must BE updated see examples. 
*/
void ERM19264_UC1609_T::LCDrotate(uint8_t rotatevalue) 
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	switch (rotatevalue)
	{
			case 0: rotatevalue = 0; break;
			case 0x02: rotatevalue = UC1609_ROTATION_FLIP_ONE; break;
			case 0x04: rotatevalue = UC1609_ROTATION_NORMAL; break;
			case 0x06: rotatevalue = UC1609_ROTATION_FLIP_TWO; break;
			default:  rotatevalue = UC1609_ROTATION_NORMAL; break;
	}
	send_command(UC1609_LCD_CONTROL, rotatevalue);
 UC1609_CS_SetHigh;
 if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	 @brief invert the display
	 @param bits 1 invert , 0 normal
*/
void ERM19264_UC1609_T::LCDinvertDisplay (uint8_t bits) 
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_INVERSE_DISPLAY, bits);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief Powerdown procedure for LCD, see datasheet P40
*/
void ERM19264_UC1609_T::LCDPowerDown(void)
{
	UC1609_RST_SetLow;
	delay(1);  // datasheet FIG 14 says >= 3uS
	UC1609_RST_SetHigh;
	LCDEnable(0);
	if(isHardwareSPI()) {SPI.end();}
}

/*!
	@brief turns on all Pixels
	@param bits Set DC[1] to force all SEG drivers to output ON signals
	1 all on ,  0 all off
*/
void ERM19264_UC1609_T::LCDAllpixelsOn(uint8_t bits) 
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_ALL_PIXEL_ON, bits);
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief Fill the screen NOT the buffer with a datapattern
	@param dataPattern can be set to zero to clear screen (not buffer) range 0x00 to 0ff
	@param delay optional delay in microseconds can be set to zero normally.
*/
void ERM19264_UC1609_T::LCDFillScreen(uint8_t dataPattern=0, uint8_t delay=0) 
{
 	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	uint16_t numofbytes = LCD_WIDTH * (LCD_HEIGHT /8); // width * height
	for (uint16_t i = 0; i < numofbytes; i++) 
	{
		send_data(dataPattern);
		delayMicroseconds(delay);
	}
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	 @brief Fill the chosen page at cursor  with a datapattern
	 @param dataPattern can be set to 0 to FF (not buffer)
*/
void ERM19264_UC1609_T::LCDFillPage(uint8_t dataPattern=0) 
{
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	uint16_t numofbytes = ((LCD_WIDTH * (LCD_HEIGHT /8))/8); // (width * height/8)/8 = 192 bytes
	for (uint16_t i = 0; i < numofbytes; i++) 
	{
			send_data(dataPattern);
	}
	UC1609_CS_SetHigh;
	if (isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	 @brief Draw a bitmap in PROGMEM to the screen
	 @param x offset 0-192
	 @param y offset 0-64
	 @param w width 0-192
	 @param h height 0-64
	 @param data  pointer to the bitmap must be in PROGMEM
	 @return 0 for success 	, negative for error
*/
int ERM19264_UC1609_T::LCDBitmap(int16_t x, int16_t y, uint8_t w, uint8_t h, const uint8_t* data) 
{
	if (data == nullptr )
	{
		return -2; // error no data nullptr
	}
	if(isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	#if defined(ESP8266)
		// ESP8266 needs a periodic yield() call to avoid watchdog reset.
		yield();
	#endif
	uint8_t tx, ty; 
	uint16_t offset = 0; 
	uint8_t column = (x < 0) ? 0 : x;
	uint8_t page = (y < 0) ? 0 : y >>3;

	for (ty = 0; ty < h; ty = ty + 8) 
	{
				if (y + ty < 0 || y + ty >= LCD_HEIGHT) {continue;}
				send_command(UC1609_SET_COLADD_LSB, (column & 0x0F)); 
				send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
				send_command(UC1609_SET_PAGEADD, page++); 

				for (tx = 0; tx < w; tx++) 
				{
							if (x + tx < 0 || x + tx >= LCD_WIDTH) {continue;}
							offset = (w * (ty >> 3)) + tx; 
							send_data(pgm_read_byte(&data[offset]));
				}
	}
	UC1609_CS_SetHigh;
	if(isHardwareSPI()) {UC_SPI_TRANSACTION_END}
	return 0; // success
}

/*!
	 @brief Checks if software SPI is on
	 @returns true 1 if hardware SPi on , false 0 for software spi
*/
bool ERM19264_UC1609_T::isHardwareSPI() {
	return (_LCD_DIN == -1 && _LCD_SCLK == -1);
}

/*!
	 @brief used in software SPI mode to shift out data
	 @param bitOrder LSB or MSB set to MSBFIRST for UC1609C
	 @param value the byte to go out 
	 @details if using high freq MCU the delay define can be changed by LCDHighFreqDelaySet function . Default is at UC1609_HIGHFREQ_DELAY
*/
void ERM19264_UC1609_T::CustomshiftOut(uint8_t bitOrder, uint8_t value)
{
		uint8_t i;

		for (i = 0; i < 8; i++)  {
				if (bitOrder == LSBFIRST)
						digitalWrite(_LCD_DIN, !!(value & (1 << i)));
				else    
						digitalWrite(_LCD_DIN, !!(value & (1 << (7 - i))));
						
				digitalWrite(_LCD_SCLK, HIGH);
				delayMicroseconds(UC1609_HIGHFREQ_DELAY);
				digitalWrite(_LCD_SCLK, LOW);
				delayMicroseconds(UC1609_HIGHFREQ_DELAY);
		}
}

/*!
	 @brief Send data byte with SPI to UC1609
	 @param byte the data byte to send 
*/
void ERM19264_UC1609_T::send_data(uint8_t byte)
{
	if (isHardwareSPI()) 
	{
		 (void)SPI.transfer(byte); // Hardware SPI
	}else
	{
		CustomshiftOut(MSBFIRST, byte); //Software SPI
	}
} 

/*!
	@brief goes to XY position
	@param column 0-192
	@param page 0-7 
*/
void ERM19264_UC1609_T::LCDGotoXY(uint8_t column , uint8_t page)
{
	if(isHardwareSPI()) {UC_SPI_TRANSACTION_START}
	UC1609_CS_SetLow;
	send_command(UC1609_SET_COLADD_LSB, (column & 0x0F)); 
	send_command(UC1609_SET_COLADD_MSB, (column & 0xF0) >> 4);
	send_command(UC1609_SET_PAGEADD, page++); 
	UC1609_CS_SetHigh;
	if(isHardwareSPI()) {UC_SPI_TRANSACTION_END}
}

/*!
	@brief Draws a single character
	@param character the character to draw, must be in the ASCII range of font
	@param stringMode if true the function will not set CS low or high, this is used for drawing strings
										called from LCDString
	@note The character is drawn using the custom font defined in LCD_FONT_CUSTOM.h
*/
void ERM19264_UC1609_T::LCDChar(unsigned char character, bool stringMode)
{
	 if (stringMode == false){
			if(isHardwareSPI()) {UC_SPI_TRANSACTION_START}
			UC1609_CS_SetLow;
	 }
		UC1609_FONTPADDING;
		for (uint8_t  column = 0 ; column <  UC1609_FONTWIDTH ; column++)
		{
				send_data((pgm_read_byte(custom_font + (((character-UC1609_ASCII_OFFSET)*UC1609_FONTWIDTH)) + column)));
		}
		UC1609_FONTPADDING;
		if (stringMode == false) {
			UC1609_CS_SetHigh;
			if(isHardwareSPI()) {UC_SPI_TRANSACTION_END}
		}
}

/*! 
	@brief Draws a string of characters
	@param characters pointer to the start of the character array
	@details The characters are drawn using the custom font defined in LCD_FONT_CUSTOM.h
		 The function will continue to draw characters until it reaches a null terminator 
	@return 0 for success, negative for error
*/
int ERM19264_UC1609_T::LCDString(const unsigned  char *characters)
{
		
		if (characters == nullptr)
		{
			return -2; // error no characters nullptr
		}
		if(isHardwareSPI()) {UC_SPI_TRANSACTION_START}
		UC1609_CS_SetLow;
		while (*characters){
			LCDChar(*characters++, true);
		}
		UC1609_CS_SetHigh;
		if(isHardwareSPI()) {UC_SPI_TRANSACTION_END}
		return 0; // success
}

//***********************************************
