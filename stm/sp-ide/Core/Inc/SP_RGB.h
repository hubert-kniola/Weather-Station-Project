#ifndef SP_RGB_h
#define SP_RGB_h

#include "main.h"

typedef enum Mode
{
	RGB_Disabled,
	RGB_White,
	RGB_Red,
	RGB_Green, 
	RGB_Blue,
	RGB_BlinkWhite,
	RGB_BlinkRed,
	RGB_BlinkGreen,
	RGB_BlinkBlue,
	RGB_Rainbow
} RGB_Mode;

void RGB_Init(void);
void RGB_Update(void);
void RGB_ToggleUsage(void);

#endif
