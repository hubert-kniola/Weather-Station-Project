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

void RGB_Init();
void RGB_SetWhite();
void RGB_SetBlack();
void RGB_SetRed();
void RGB_SetGreen();
void RGB_SetBlue();
void RGB_Rainbow();
void RGB_BlinkRed();
void RGB_BlinkGreen();
void RGB_BlinkBlue();
void RGB_BlinkWhite();

void _RGB_UpdateRegisters();
void _RGB_Test(uint8_t choice);

#endif