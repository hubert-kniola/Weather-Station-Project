#include "SP_LCD.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// Polecenia sterownika HITACHI HD44780 uzywane w module
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

#define LCD_DISPLAYMOVE 0x08
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_5x8DOTS 0x00

#define LCD_COLUMNS 20
#define LCD_ROWS 4

#define LCD_PORT GPIOE

uint16_t _rs;               // LOW: funkcja,  HIGH: dane
uint16_t _rw;               // LOW: zapis,  HIGH: odczyt
uint16_t _enable;
uint16_t _data[4];

uint8_t _displayFunction;
uint8_t _displayControl;
uint8_t _displayMode;
uint8_t _initialized;
uint8_t _rowOffsets[4];

uint8_t _currentRow;
uint8_t _currentCol;


void LCD_Init(void)
{
	_rs = GPIO_PIN_4;
	_rw = GPIO_PIN_5;
	_enable = GPIO_PIN_6;
  
	_data[0] = GPIO_PIN_0;
	_data[1] = GPIO_PIN_1;
	_data[2] = GPIO_PIN_2;
	_data[3] = GPIO_PIN_3; 

	_displayFunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
  
	/* Start z wlaczonym podswietleniem */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	_LCD_SetRowOffsets(0x00, 0x40, 0x00 + LCD_COLUMNS, 0x40 + LCD_COLUMNS);  

	/* Opoznienia sa w celu ustablizowania napiec na diodach ekranu */
	HAL_Delay(50); 

	HAL_GPIO_WritePin(LCD_PORT, _rs, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_PORT, _enable, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_PORT, _rw, GPIO_PIN_RESET);
	
	/* procedura inicjalizacji dzialania na 4 bitach danych */
	_LCD_WriteData(0x03);
	HAL_Delay(5);

	_LCD_WriteData(0x03);
	HAL_Delay(5);
    
	_LCD_WriteData(0x03); 
	HAL_Delay(1);

	_LCD_WriteData(0x02); 
	_LCD_SendCommand(LCD_FUNCTIONSET | _displayFunction);  
	_displayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  

	LCD_DisplayOn();
	LCD_ClearScreen();

	/* Domyslna forma tekstu */
	_displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	_LCD_SendCommand(LCD_ENTRYMODESET | _displayMode);
	
	_currentRow = 0;
	_currentCol = 0;
	
	LCD_PrintCentered("booting...");
}

void LCD_ToggleBackgroundLED(void)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);
}

void _LCD_SetRowOffsets(int row0, int row1, int row2, int row3)
{
	_rowOffsets[0] = row0;
	_rowOffsets[1] = row1;
	_rowOffsets[2] = row2;
	_rowOffsets[3] = row3;
}

void LCD_ClearScreen(void)
{
	_LCD_SendCommand(LCD_CLEARDISPLAY); 
	HAL_Delay(2); 
	LCD_ResetCursor();
}

void LCD_ResetCursor(void)
{
	_LCD_SendCommand(LCD_RETURNHOME);
	HAL_Delay(2);
	LCD_SetCursor(0, 0);
}

void LCD_SetCursor(uint8_t col, uint8_t row)
{
	/* Aby nigdy nie wyjsc poza zakres tablicy */
	if (row >= LCD_ROWS) {
		row = LCD_ROWS - 1;
	}
  
	_LCD_SendCommand(LCD_SETDDRAMADDR | (col + _rowOffsets[row]));
	_currentRow = row;
	_currentCol = col;
}

void LCD_DisplayOff(void) {
	_displayControl &= ~LCD_DISPLAYON;
	_LCD_SendCommand(LCD_DISPLAYCONTROL | _displayControl);
}
void LCD_DisplayOn(void) {
	_displayControl |= LCD_DISPLAYON;
	_LCD_SendCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_DisableCursor(void) {
	_displayControl &= ~LCD_CURSORON;
	_LCD_SendCommand(LCD_DISPLAYCONTROL | _displayControl);
}
void LCD_EnableCursor(void) {
	_displayControl |= LCD_CURSORON;
	_LCD_SendCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_DisableBlink(void) {
	_displayControl &= ~LCD_BLINKON;
	_LCD_SendCommand(LCD_DISPLAYCONTROL | _displayControl);
}
void LCD_EnableBlink(void) {
	_displayControl |= LCD_BLINKON;
	_LCD_SendCommand(LCD_DISPLAYCONTROL | _displayControl);
}

void LCD_ScrollOneLeft(void) {
	_LCD_SendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LCD_ScrollOneRight(void) {
	_LCD_SendCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LCD_AlignLeft(void) {
	_displayMode |= LCD_ENTRYLEFT;
	_LCD_SendCommand(LCD_ENTRYMODESET | _displayMode);
}

void LCD_AlignRight(void) {
	_displayMode &= ~LCD_ENTRYLEFT;
	_LCD_SendCommand(LCD_ENTRYMODESET | _displayMode);
}

void LCD_EnableAutoscroll(void) {
	_displayMode |= LCD_ENTRYSHIFTINCREMENT;
	_LCD_SendCommand(LCD_ENTRYMODESET | _displayMode);
}

void LCD_DisableAutoscroll(void) {
	_displayMode &= ~LCD_ENTRYSHIFTINCREMENT;
	_LCD_SendCommand(LCD_ENTRYMODESET | _displayMode);
}

void LCD_Print(const char str[]) {
	if (str == NULL) return;

	const uint8_t *buffer = (const uint8_t *)str;
	size_t size = strlen(str);

	for (int i = 0; i < size; i++) {
		_LCD_SendData(buffer[i], true);
	}
}

void LCD_Printf(const char format[], ...)
{
	va_list va;
	va_start(va, format);
	
	char str[80];
	const int size = vsnprintf(str, (uint8_t) - 1, format, va);
	
	va_end(va);
	
	if (size > 80) return;
	
	LCD_Print(str);
}

void LCD_PrintfDelayed(uint16_t delay, const char format[], ...)
{
	va_list va;
	va_start(va, format);
	
	char str[80];
	const int size = vsnprintf(str, (uint8_t) - 1, format, va);
	
	va_end(va);
	
	if (size > 80) return;
	
	for (int i = 0; i < size; i++) {
		_LCD_SendData(((const uint8_t *)str)[i], true);
		HAL_Delay(delay);
	}
}

void LCD_PrintCentered(const char str[])
{
	const int size = strlen(str);
	if (size > 20) return;
	
	_currentCol = (int)((LCD_COLUMNS - size) / 2);
	
	LCD_SetCursor(_currentCol, _currentRow);
	LCD_Print(str);
}

void LCD_NextLine(void)
{
	_currentCol = 0;
	++_currentRow;
			
	if (_currentRow >= LCD_ROWS)
	{
		_currentRow = 0;
	}
	
	LCD_SetCursor(_currentCol, _currentRow);
}

void LCD_DefineCustomChar(uint8_t location, uint8_t bytes[]) {
	/* 8 miejsc do zapisu 0-7 */
	location &= 0x7; /* zawsze bezpieczny adres */ 
	_LCD_SendCommand(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) {
		_LCD_SendData(bytes[i], false);
	}
}

void _LCD_SendCommand(uint8_t value) {
	_LCD_SendByteWithState(value, GPIO_PIN_RESET);
}

void _LCD_SendData(uint8_t value, bool moveCursor) {
	_LCD_SendByteWithState(value, GPIO_PIN_SET);
	
	if (moveCursor)
	{
		++_currentCol;
		if (_currentCol >= LCD_COLUMNS)
		{
			_currentCol = 0;
			++_currentRow;
			
			if (_currentRow >= LCD_ROWS)
			{
				_currentRow = 0;
			}
			LCD_SetCursor(_currentCol, _currentRow);
		}
	}
}

void _LCD_SendByteWithState(uint8_t value, GPIO_PinState mode) {
	HAL_GPIO_WritePin(LCD_PORT, _rs, mode);

	if (_rw != 255) { 
		HAL_GPIO_WritePin(LCD_PORT, _rw, GPIO_PIN_RESET);
	}
  
	_LCD_WriteData(value >> 4);
	_LCD_WriteData(value);
}

void _LCD_EnableSignal(void) {
	HAL_GPIO_WritePin(LCD_PORT, _enable, GPIO_PIN_RESET);
	HAL_Delay(1);    
	HAL_GPIO_WritePin(LCD_PORT, _enable, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_PORT, _enable, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void _LCD_WriteData(uint8_t value) {
	for (int i = 0; i < 4; i++) {
		/* Little Endian */
		HAL_GPIO_WritePin(LCD_PORT, _data[i], ((value >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	_LCD_EnableSignal();
}
