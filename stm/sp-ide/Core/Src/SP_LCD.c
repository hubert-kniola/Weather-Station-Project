#include "SP_LCD.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

// Polecenia sterownika HITACHI HD44780 uzywane w module
#define CLEAR_DISPLAY 		0x01
#define RETURN_HOME 		0x02
#define ENTRY_MODESET 		0x04
#define DISPLAY_CONTROL 	0x08
#define CURSOR_SHIFT 		0x10
#define FUNCTION_SET 		0x20
#define SET_CGRAM_ADDR 		0x40
#define SET_DDRAM_ADDR 		0x80

#define ENTRY_RIGHT 			0x00
#define ENTRY_LEFT 				0x02
#define ENTRY_SHIFT_INCREMENT 	0x01
#define ENTRY_SHIFT_DECREMENT 	0x00

#define DISPLAY_ON 		0x04
#define DISPLAY_OFF 	0x00
#define CURSOR_ON 		0x02
#define CURSOR_OFF 		0x00
#define BLINK_ON 		0x01
#define BLINK_OFF 		0x00

#define DISPLAY_MOVE 	0x08
#define MOVE_RIGHT 		0x04
#define MOVE_LEFT 		0x00

#define FOUR_BIT_MODE 	0x00
#define TWO_LINE 		0x08
#define TWENTY_DOTS 	0x00

#define COLUMNS 	20
#define ROWS 		4

#define DEG_CHAR 	7
#define TIN_CHAR 	6
#define TOUT_CHAR 	5
#define HIN_CHAR 	4
#define HOUT_CHAR 	3

/* Port glowny i piny */
#define PORT 	GPIOE
#define RS_PIN GPIO_PIN_4
#define RW_PIN GPIO_PIN_5
#define EN_PIN GPIO_PIN_6
#define D4_PIN GPIO_PIN_0
#define D5_PIN GPIO_PIN_1
#define D6_PIN GPIO_PIN_2
#define D7_PIN GPIO_PIN_3

/* Port i pin podswietlenia */
#define BG_PORT 	GPIOC
#define BG_PIN 		GPIO_PIN_15

extern TIM_HandleTypeDef htim3; /* uzyj numeru timera w LCD_WakeScreen */

uint16_t _readyData[4];

uint8_t _rowOffsets[4];

uint8_t _displayFunction;
uint8_t _displayControl;
uint8_t _displayMode;

uint8_t _currentRow;
uint8_t _currentCol;

void _LCD_SetRowOffsets(int row0, int row1, int row2, int row3) {
	_rowOffsets[0] = row0;
	_rowOffsets[1] = row1;
	_rowOffsets[2] = row2;
	_rowOffsets[3] = row3;
}

void _LCD_EnableSignal(void) {
	HAL_GPIO_WritePin(PORT, EN_PIN, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(PORT, EN_PIN, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(PORT, EN_PIN, GPIO_PIN_RESET);
	HAL_Delay(1);
}

void _LCD_WriteData(uint8_t value) {
	for (int i = 0; i < 4; i++) {
		/* Little Endian */
		HAL_GPIO_WritePin(PORT, _readyData[i],
				((value >> i) & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}

	_LCD_EnableSignal();
}

void _LCD_SendByteWithState(uint8_t value, GPIO_PinState mode) {
	HAL_GPIO_WritePin(PORT, RS_PIN, mode);

	if (RW_PIN != 255) {
		HAL_GPIO_WritePin(PORT, RW_PIN, GPIO_PIN_RESET);
	}

	_LCD_WriteData(value >> 4);
	_LCD_WriteData(value);
}

void _LCD_SendCommand(uint8_t value) {
	_LCD_SendByteWithState(value, GPIO_PIN_RESET);
}

void _LCD_SendData(uint8_t value, bool moveCursor) {
	_LCD_SendByteWithState(value, GPIO_PIN_SET);

	if (moveCursor) {
		++_currentCol;
		if (_currentCol >= COLUMNS) {
			_currentCol = 0;
			++_currentRow;

			if (_currentRow >= ROWS) {
				_currentRow = 0;
			}
			LCD_SetCursor(_currentCol, _currentRow);
		}
	}
}

void LCD_Init(void) {
	/* Piny danych */
	_readyData[0] = D4_PIN;
	_readyData[1] = D5_PIN;
	_readyData[2] = D6_PIN;
	_readyData[3] = D7_PIN;

	_displayFunction = FOUR_BIT_MODE | TWO_LINE | TWENTY_DOTS;

	/* Start z wlaczonym podswietleniem */
	LCD_BackgroundOn();
	_LCD_SetRowOffsets(0x00, 0x40, 0x00 + COLUMNS, 0x40 + COLUMNS);

	/* Opoznienia sa w celu ustablizowania napiec na diodach ekranu */
	HAL_Delay(50);

	HAL_GPIO_WritePin(PORT, RS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, EN_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PORT, RW_PIN, GPIO_PIN_RESET);

	/* procedura inicjalizacji dzialania na 4 bitach danych */
	_LCD_WriteData(0x03);
	HAL_Delay(5);

	_LCD_WriteData(0x03);
	HAL_Delay(5);

	_LCD_WriteData(0x03);
	HAL_Delay(1);

	_LCD_WriteData(0x02);
	_LCD_SendCommand(FUNCTION_SET | _displayFunction);
	_displayControl = DISPLAY_ON | CURSOR_OFF | BLINK_OFF;

	LCD_DisplayOn();
	LCD_ClearScreen();

	/* Domyslna forma tekstu */
	_displayMode = ENTRY_LEFT | ENTRY_SHIFT_DECREMENT;
	_LCD_SendCommand(ENTRY_MODESET | _displayMode);

	_currentRow = 0;
	_currentCol = 0;

	/* welcome screen */
	LCD_PrintCentered("Weather  Station");
	LCD_SetCursor(0, 2);
	LCD_PrintCentered("v1.0");
	LCD_SetCursor(0, 3);
	LCD_PrintCentered("2020 @ PUT");

	uint8_t char7[8] = { 0b11000, 0b11000, 0b00110, 0b01001, 0b01000, 0b01000,
			0b01001, 0b00110 };
	LCD_DefineCustomChar(DEG_CHAR, char7); /* Stopnie */

	uint8_t char6[8] = { 0b11100, 0b01000, 0b01001, 0b00000, 0b01000, 0b01001,
			0b11100, 0b01000 };
	LCD_DefineCustomChar(TIN_CHAR, char6); /* Temp IN */

	uint8_t char5[8] = { 0b11100, 0b01000, 0b01001, 0b00000, 0b01000, 0b11101,
			0b01000, 0b01000 };
	LCD_DefineCustomChar(TOUT_CHAR, char5); /* Temp OUT */

	uint8_t char4[8] = { 0b10100, 0b11100, 0b10101, 0b00000, 0b01000, 0b01001,
			0b11100, 0b01000 };
	LCD_DefineCustomChar(HIN_CHAR, char4); /* Humid IN */

	uint8_t char3[8] = { 0b10100, 0b11100, 0b10101, 0b00000, 0b01000, 0b11101,
			0b01000, 0b01000 };
	LCD_DefineCustomChar(HOUT_CHAR, char3); /* Humid OUT */
}

void LCD_WakeScreen(void) {
	LCD_BackgroundOn();
	ResetTIM(3);
}

void LCD_BackgroundOn(void) {
	HAL_GPIO_WritePin(BG_PORT, BG_PIN, GPIO_PIN_SET);
}

void LCD_BackgroundOff(void) {
	HAL_GPIO_WritePin(BG_PORT, BG_PIN, GPIO_PIN_RESET);
}

void LCD_ClearScreen(void) {
	_LCD_SendCommand(CLEAR_DISPLAY);
	LCD_ResetCursor();
}

void LCD_ResetCursor(void) {
	LCD_SetCursor(0, 0);
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
	if (row >= ROWS) {
		row = ROWS - 1;
	}
	if (col >= COLUMNS) {
		col = COLUMNS - 1;
	}

	_LCD_SendCommand(SET_DDRAM_ADDR | (col + _rowOffsets[row]));
	_currentRow = row;
	_currentCol = col;
}

void LCD_DisplayOff(void) {
	_displayControl &= ~DISPLAY_ON;
	_LCD_SendCommand(DISPLAY_CONTROL | _displayControl);
}
void LCD_DisplayOn(void) {
	_displayControl |= DISPLAY_ON;
	_LCD_SendCommand(DISPLAY_CONTROL | _displayControl);
}

void LCD_DisableCursor(void) {
	_displayControl &= ~CURSOR_ON;
	_LCD_SendCommand(DISPLAY_CONTROL | _displayControl);
}
void LCD_EnableCursor(void) {
	_displayControl |= CURSOR_ON;
	_LCD_SendCommand(DISPLAY_CONTROL | _displayControl);
}

void LCD_DisableBlink(void) {
	_displayControl &= ~BLINK_ON;
	_LCD_SendCommand(DISPLAY_CONTROL | _displayControl);
}
void LCD_EnableBlink(void) {
	_displayControl |= BLINK_ON;
	_LCD_SendCommand(DISPLAY_CONTROL | _displayControl);
}

void LCD_ScrollOneLeft(void) {
	_LCD_SendCommand(CURSOR_SHIFT | DISPLAY_MOVE | MOVE_LEFT);
}
void LCD_ScrollOneRight(void) {
	_LCD_SendCommand(CURSOR_SHIFT | DISPLAY_MOVE | MOVE_RIGHT);
}

void LCD_AlignLeft(void) {
	_displayMode |= ENTRY_LEFT;
	_LCD_SendCommand(ENTRY_MODESET | _displayMode);
}

void LCD_AlignRight(void) {
	_displayMode &= ~ENTRY_LEFT;
	_LCD_SendCommand(ENTRY_MODESET | _displayMode);
}

void LCD_EnableAutoscroll(void) {
	_displayMode |= ENTRY_SHIFT_INCREMENT;
	_LCD_SendCommand(ENTRY_MODESET | _displayMode);
}

void LCD_DisableAutoscroll(void) {
	_displayMode &= ~ENTRY_SHIFT_INCREMENT;
	_LCD_SendCommand(ENTRY_MODESET | _displayMode);
}

void LCD_Print(const char str[]) {
	if (str == NULL)
		return;

	const uint8_t *buffer = (const uint8_t*) str;
	size_t size = strlen(str);

	for (int i = 0; i < size; i++) {
		_LCD_SendData(buffer[i], true);
	}
}

void LCD_PrintCentered(const char str[]) {
	const int size = strlen(str);
	if (size > 20)
		return;

	_currentCol = (int) ((COLUMNS - size) / 2);

	LCD_SetCursor(_currentCol, _currentRow);
	LCD_Print(str);
}

void LCD_PrintTempInfo(float *data1, float *data2) {
	if (data1 == NULL)
		return;

	char temp[10], rh[10];

	/* 1 sensor */
	if (data1[0] < 10.f)
		sprintf(temp, "\6 %.0f\7", data1[0]);
	else
		sprintf(temp, "\6%.0f\7", data1[0]);

	if (data1[1] < 10.f)
		sprintf(rh, "\4 %.0f%%", data1[1]);
	else if (data1[1] == 100.f)
		sprintf(rh, "\499%%");
	else
		sprintf(rh, "\4%.0f%%", data1[1]);

	if (data2 != NULL) {
		LCD_SetCursor(0, 1);
		LCD_Print(temp);
		LCD_SetCursor(0, 2);
		LCD_Print(rh);
		/* 2 sensor */
		if (data2[0] < 10.f)
			sprintf(temp, "\5 %.0f\7", data2[0]);
		else
			sprintf(temp, "\5%.0f\7", data2[0]);

		if (data2[1] < 10.f)
			sprintf(rh, "\3 %.0f%%", data2[1]);
		else if (data2[1] == 100.f)
			sprintf(rh, "\399%%");
		else
			sprintf(rh, "\3%.0f%%", data2[1]);

		LCD_SetCursor(16, 1);
		LCD_Print(temp);
		LCD_SetCursor(16, 2);
		LCD_Print(rh);
	} else {
		LCD_SetCursor(0, 1);
		LCD_PrintCentered(temp);
		LCD_NextLine("");
		LCD_PrintCentered(rh);
	}
}

void LCD_NextLine(const char text[]) {
	LCD_Print(text);

	_currentCol = 0;
	++_currentRow;

	if (_currentRow >= ROWS) {
		_currentRow = 0;
	}

	LCD_SetCursor(_currentCol, _currentRow);
}

uint8_t LCD_CursorUp(void) {
	if (_currentRow > 0)
		--_currentRow;

	LCD_SetCursor(_currentCol, _currentRow);
	return _currentRow;
}
uint8_t LCD_CursorDown(void) {
	if (_currentRow < ROWS)
		++_currentRow;

	LCD_SetCursor(_currentCol, _currentRow);
	return _currentRow;
}
uint8_t LCD_CursorLeft(void) {
	if (_currentCol > 0) {
		--_currentCol;
	} else {
		_currentCol = COLUMNS - 1;
		if (_currentRow == 0) {
			_currentRow = ROWS - 1;
		} else {
			--_currentRow;
		}
	}
	LCD_SetCursor(_currentCol, _currentRow);
	return _currentCol;
}
uint8_t LCD_CursorRight(void) {
	if (_currentCol < COLUMNS - 1) {
		++_currentCol;
	} else {
		_currentCol = 0;
		if (_currentRow == ROWS - 1) {
			_currentRow = 0;
		} else {
			++_currentRow;
		}
	}
	LCD_SetCursor(_currentCol, _currentRow);
	return _currentCol;
}

void LCD_DefineCustomChar(uint8_t location, uint8_t bytes[]) {
	/* 8 miejsc do zapisu 0-7 */
	location &= 0x7; /* zawsze bezpieczny adres */
	_LCD_SendCommand(SET_CGRAM_ADDR | (location << 3));
	for (int i = 0; i < 8; i++) {
		_LCD_SendData(bytes[i], false);
	}
}

void LCD_PrintDateTime(const char date[], const char time[]) {
	LCD_SetCursor(0, 0);
	LCD_Print(date);
	LCD_SetCursor(15, 0);
	LCD_Print(time);
	LCD_NextLine("");
}

void LCD_WriteChar(char character) {
	_LCD_SendData((uint8_t) character, true);
	LCD_CursorLeft();
}

void LCD_PrintNetworks(char *data, int from) {
	int index = 0;
	int column = 0;
	char number = from + '0';

	/* ustaw indeks na pozadanej pozycji */
	for (int i = 0; i < from - 1; i++) {
		while (data[index++] != ';')
			;
	}

	LCD_ClearScreen();

	/* wypisujemy maks 4 */
	for (int i = 1; i <= 4; i++) {
		LCD_WriteChar(number++);
		column = LCD_CursorRight();
		LCD_Print(":");

		while (data[index] != ';') {
			if (column > 0) { /* kursor skoczyl do nowej linii */
				LCD_WriteChar(data[index]);
				column = LCD_CursorRight();
			}
			index++;
		}

		if (data[index + 1] == 0) {
			break;
		} else {
			if (column != 0)
				LCD_NextLine("");
			index++;
		}
	}
}

void LCD_PrintOptionsScreen(const char string[], int from) {
	int index = 0;
	for (int i = 0; i < from - 1; i++) {
		while (string[index++] != ';')
			;
	}

	LCD_ClearScreen();
	LCD_PrintCentered("Options:");

	for (int i = 1; i <= 3; i++) {
		LCD_SetCursor(0, i);

		while (string[index] != ';') {
			LCD_WriteChar(string[index++]);
			LCD_CursorRight();
		}
		index++;
	}
}

void LCD_PrintNetworkStatus(ModeEnum mode, char *data) {
	/* wyczysc linie */
	LCD_SetCursor(0, 3);
	LCD_Print("                    ");
	LCD_SetCursor(0, 3);

	if (mode == MD_ClientDConn) {
		LCD_PrintCentered("No WiFi Connection");
	} else if (mode == MD_ClientConn) {
		if (data == NULL)
			return;

		int size = strlen(data);
		_currentCol = (int) ((COLUMNS - size) / 2);

		LCD_SetCursor(_currentCol, _currentRow);
		for (int i = 0; i < size; i++) {
			LCD_WriteChar(data[i]);
			LCD_CursorRight();
		}
	} else if (mode == MD_AccessPoint) {

	} else if (mode == MD_LostHost) {
		LCD_PrintCentered("Network unavailable!");
	}
}

void LCD_FatalSDScreen(void) {
	LCD_ClearScreen();

	LCD_PrintCentered("Fatal error, sd card");
	LCD_NextLine("");
	LCD_PrintCentered("must be formated");
	LCD_NextLine("");
	LCD_PrintCentered("Copy HTTP files manu");
	LCD_NextLine("");
	LCD_PrintCentered("ally to fix device");

	HAL_Delay(1000);
}
