#ifndef SP_RGB_h
#define SP_RGB_h

#include "main.h"

enum RGB_Mode
{
	Disabled,
	White,
	Red,
	Green, 
	Blue,
	BlinkWhite,
	BlinkRed,
	BlinkGreen,
	BlinkBlue,
	Rainbow,
	NONE
};

typedef enum RGB_Mode RGB_Mode;

void RGB_Init(void);

void RGB_SetWhite(void);
void RGB_SetBlack(void);
void RGB_SetRed(void);
void RGB_SetGreen(void);
void RGB_SetBlue(void);

void RGB_Rainbow(void);
void RGB_BlinkRed(void);
void RGB_BlinkGreen(void);
void RGB_BlinkBlue(void);
void RGB_BlinkWhite(void);

void _RGB_UpdateRegisters(void);
void _RGB_Test(uint8_t);

#endif