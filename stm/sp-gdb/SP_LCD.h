#ifndef SP_LCD_h
#define SP_LCD_h
#include "main.h"

// low-level functions
void send(uint8_t, GPIO_PinState);
void write4bits(uint8_t);
void write8bits(uint8_t);
void pulseEnable(void);

// initializers
void LCD_Init();
void LCD_ToggleScreen();

void begin(uint8_t cols, uint8_t rows);

// high-level functions
void clear(void);
void home(void);

void noDisplay(void);
void display(void);
void noBlink(void);
void blink(void);
void noCursor(void);
void cursor(void);
void scrollDisplayLeft(void);
void scrollDisplayRight(void);
void leftToRight(void);
void rightToLeft(void);
void autoscroll(void);
void noAutoscroll(void);
size_t print(const char[]);

void setRowOffsets(int row1, int row2, int row3, int row4);
void createChar(uint8_t, uint8_t[]);
void setCursor(uint8_t, uint8_t); 
size_t write(uint8_t);
void command(uint8_t);

#endif