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

void LCD_NextLine(const char[]);
void LCD_Print(const char[]);
void LCD_Printf(const char[], ...);
void LCD_PrintfDelayed(uint16_t, const char[], ...);
void LCD_PrintCentered(const char[]);
void LCD_PrintDegree(void);
void LCD_PrintTempInfo(float*, float*);
void LCD_PrintDateTime(const char[], const char[]);

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

#endif
