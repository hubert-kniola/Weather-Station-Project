#pragma once
#include "main.h"

enum RGB_Mode
{
	/* Still Modes */
	SN,
	SW,
	SR,
	SG, 
	SB,
	/* Blinking Modes */
	BW,
	BR,
	BG,
	BB,
	RR,
	NONE
};

typedef enum RGB_Mode Mode;

struct RGB_Data_Handle
{
	uint32_t _CCR1; /* RED */
	uint32_t _CCR2; /* GREEN */
	uint32_t _CCR3; /* BLUE */
	
	uint16_t _state;
	Mode _mode;
};

typedef struct RGB_Data_Handle RGB_Handler;

void RGB_Init();
void RGB_SetWhite();
void RGB_SetBlack();
void RGB_SetRed();
void RGB_SetGreen();
void RGB_SetBlue();
void _RGB_UpdateRegisters();
void RGB_Rainbow();
void RGB_BlinkRed();
void RGB_BlinkGreen();
void RGB_BlinkBlue();
void RGB_BlinkWhite();
void _RGB_Test(uint8_t choice);