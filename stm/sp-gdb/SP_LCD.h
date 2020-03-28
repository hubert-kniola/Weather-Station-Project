#ifndef SP_LCD_h
#define SP_LCD_h

#include "main.h"
#include <stdbool.h>

void LCD_Init(void);

void LCD_ToggleBackgroundLED(void);
void LCD_ClearScreen(void);
void LCD_ResetCursor(void);
void LCD_DisplayOff(void);
void LCD_DisplayOn(void);

void LCD_NextLine(void);
void LCD_Print(const char[]);
void LCD_Printf(const char[], ...);
void LCD_PrintfDelayed(uint16_t, const char[], ...);
void LCD_PrintCentered(const char[]);

void LCD_DisableBlink(void);
void LCD_EnableBlink(void);
void LCD_DisableCursor(void);
void LCD_EnableCursor(void);
void LCD_ScrollOneLeft(void);
void LCD_ScrollOneRight(void);
void LCD_AlignLeft(void);
void LCD_AlignRight(void);
void LCD_EnableAutoscroll(void);
void LCD_DisableAutoscroll(void);
void LCD_DefineCustomChar(uint8_t, uint8_t[]);
void LCD_SetCursor(uint8_t, uint8_t); 

void _LCD_SetRowOffsets(int, int, int, int);
void _LCD_SendData(uint8_t, bool);
void _LCD_SendCommand(uint8_t);
void _LCD_SendByteWithState(uint8_t, GPIO_PinState);
void _LCD_WriteData(uint8_t);
void _LCD_EnableSignal(void);

#endif