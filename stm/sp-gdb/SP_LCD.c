/*
 * LiquidCrystal.c - LiquidCrystal Library for STM32 ARM microcontrollers
 *
 *  Created on: April 12, 2018
 *      Author: S. Saeed Hosseini (sayidhosseini@hotmail.com)
 *      Ported from: Arduino, Adafruit (https://github.com/arduino-libraries/LiquidCrystal)
 *      Published to: Github (https://github.com/SayidHosseini/STM32LiquidCrystal)
 */

#include "SP_LCD.h"
#include <stdio.h>
#include <string.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// Constants
#define LCD_COLUMNS 20
#define LCD_ROWS 4

// change this to LCD_5x10DOTS for some 1 line displays that can select a 10 pixel high font
uint8_t _dotSize;

// pin definitions and other LCD variables
uint16_t _rs;   // LOW: command.  HIGH: character.
uint16_t _rw;   // LOW: write to LCD.  HIGH: read from LCD.
uint16_t _enable;   // activated by a HIGH pulse.
uint16_t _data[8];
GPIO_TypeDef *_port;

uint8_t _displayFunction;
uint8_t _displayControl;
uint8_t _displayMode;

uint8_t _initialized;

uint8_t _numLines;
uint8_t _rowOffsets[4];

void LCD_Init()
{
	_dotSize = LCD_5x8DOTS;
	
	_rs = GPIO_PIN_4;
	_rw = GPIO_PIN_5;
	_enable = GPIO_PIN_6;
	_port = GPIOE;
  
	_data[0] = GPIO_PIN_0;
	_data[1] = GPIO_PIN_1;
	_data[2] = GPIO_PIN_2;
	_data[3] = GPIO_PIN_3; 
	_data[4] = 0;
	_data[5] = 0;
	_data[6] = 0;
	_data[7] = 0;

	_displayFunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  
	begin(LCD_COLUMNS, LCD_ROWS);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	
	noCursor();
}

void LCD_ToggleScreen()
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
}

void begin(uint8_t cols, uint8_t lines) {
	if (lines > 1) {
		_displayFunction |= LCD_2LINE;
	}
	_numLines = lines;

	setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);  

	// for some 1 line displays you can select a 10 pixel high font
	if((_dotSize != LCD_5x8DOTS) && (lines == 1)) {
		_displayFunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// so we'll wait 50 just to make sure
	HAL_Delay(50); 

	// Now we pull both RS and R/W low to begin commands
	HAL_GPIO_WritePin(_port, _rs, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(_port, _enable, GPIO_PIN_RESET);

	if (_rw != 255) { 
		HAL_GPIO_WritePin(_port, _rw, GPIO_PIN_RESET);
	}
  
	//put the LCD into 4 bit or 8 bit mode
	if(!(_displayFunction & LCD_8BITMODE)) {
		// this is according to the hitachi HD44780 datasheet
		// figure 24, pg 46

		// we start in 8bit mode, try to set 4 bit mode
		write4bits(0x03);
		HAL_Delay(5);   // wait min 4.1ms

		// second try
		write4bits(0x03);
		HAL_Delay(5);   // wait min 4.1ms
    
		// third go!
		write4bits(0x03); 
		HAL_Delay(1);

		// finally, set to 4-bit interface
		write4bits(0x02); 
	} else {
		// this is according to the hitachi HD44780 datasheet
		// page 45 figure 23

		// Send function set command sequence
		command(LCD_FUNCTIONSET | _displayFunction);
		HAL_Delay(5);    // wait more than 4.1ms

		// second try
		command(LCD_FUNCTIONSET | _displayFunction);
		HAL_Delay(1);

		// third go
		command(LCD_FUNCTIONSET | _displayFunction);
	}

	// finally, set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayFunction);  

	// turn the display on with no cursor or blinking default
	_displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for romance languages)
	_displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	command(LCD_ENTRYMODESET | _displayMode);

}

void setRowOffsets(int row0, int row1, int row2, int row3)
{
	_rowOffsets[0] = row0;
	_rowOffsets[1] = row1;
	_rowOffsets[2] = row2;
	_rowOffsets[3] = row3;
}

/********** high level commands, for the user! */
void clear(void)
{
	command(LCD_CLEARDISPLAY);    // clear display, set cursor position to zero
	HAL_Delay(2);    // this command takes a long time!
}

void home(void)
{
	command(LCD_RETURNHOME);    // set cursor position to zero
	HAL_Delay(2);    // this command takes a long time!
}

void setCursor(uint8_t col, uint8_t row)
{
	const size_t max_lines = sizeof(_rowOffsets) / sizeof(*_rowOffsets);
	if (row >= max_lines) {
		row = max_lines - 1;      // we count rows starting w/0
	}
	if (row >= _numLines) {
		row = _numLines - 1;      // we count rows starting w/0
	}
  
	command(LCD_SETDDRAMADDR | (col + _rowOffsets[row]));
}

// Turn the display on/off (quickly)
void noDisplay(void) {
	_displayControl &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displayControl);
}
void display(void) {
	_displayControl |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displayControl);
}

// Turns the underline cursor on/off
void noCursor(void) {
	_displayControl &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displayControl);
}
void cursor(void) {
	_displayControl |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displayControl);
}

// Turn on and off the blinking cursor
void noBlink(void) {
	_displayControl &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displayControl);
}
void blink(void) {
	_displayControl |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displayControl);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void) {
	_displayMode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displayMode);
}

// This is for text that flows Right to Left
void rightToLeft(void) {
	_displayMode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displayMode);
}

// This will 'right justify' text from the cursor
void autoscroll(void) {
	_displayMode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displayMode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void) {
	_displayMode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displayMode);
}

// This will print character string to the LCD
size_t print(const char str[]) {
	if (str == NULL) return 0;

	const uint8_t *buffer = (const uint8_t *)str;
	size_t size = strlen(str);
	size_t n = 0;

	while (size--) {
		if (write(*buffer++)) n++;
		else break;
	}
	return n;
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7;   // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) {
		write(charmap[i]);
	}
}

/*********** mid level commands, for sending data/cmds */

inline void command(uint8_t value) {
	send(value, GPIO_PIN_RESET);
}

inline size_t write(uint8_t value) {
	send(value, GPIO_PIN_SET);
	return 1; // assume sucess
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void send(uint8_t value, GPIO_PinState mode) {
	HAL_GPIO_WritePin(_port, _rs, mode);

	// if there is a RW pin indicated, set it low to Write
	if(_rw != 255) { 
		HAL_GPIO_WritePin(_port, _rw, GPIO_PIN_RESET);
	}
  
	if (_displayFunction & LCD_8BITMODE) {
		write8bits(value); 
	}
	else {
		write4bits(value >> 4);
		write4bits(value);
	}
}

void pulseEnable(void) {
	HAL_GPIO_WritePin(_port, _enable, GPIO_PIN_RESET);
	HAL_Delay(1);    
	HAL_GPIO_WritePin(_port, _enable, GPIO_PIN_SET);
	HAL_Delay(1);      // enable pulse must be >450ns
	HAL_GPIO_WritePin(_port, _enable, GPIO_PIN_RESET);
	HAL_Delay(1);     // commands need > 37us to settle
}

void write4bits(uint8_t value) {
	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(_port, _data[i], ((value >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	pulseEnable();
}

void write8bits(uint8_t value) {
	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(_port, _data[i], ((value >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
  
	pulseEnable();
}