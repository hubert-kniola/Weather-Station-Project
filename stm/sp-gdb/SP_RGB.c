#include "SP_RGB.h"

#define RGB_MAX_REGISTER_VAL 999

RGB_Handler h;

void RGB_Init()
{
	h._CCR1 = 0;
	h._CCR2 = 0;
	h._CCR3 = 0;
	h._state = 0;
	h._mode = NONE;
}
void RGB_SetWhite()
{
	h._mode = SW;
	
	h._CCR1 = RGB_MAX_REGISTER_VAL;
	h._CCR2 = RGB_MAX_REGISTER_VAL;
	h._CCR3 = RGB_MAX_REGISTER_VAL;
	
	_RGB_UpdateRegisters();
}
void RGB_SetRed()
{
	h._mode = SR;
	
	h._CCR1 = RGB_MAX_REGISTER_VAL;
	h._CCR2 = 0;
	h._CCR3 = 0;
	
	_RGB_UpdateRegisters();
}
void RGB_SetGreen()
{
	h._mode = SG;
	
	h._CCR1 = 0;
	h._CCR2 = RGB_MAX_REGISTER_VAL;
	h._CCR3 = 0;
	
	_RGB_UpdateRegisters();
}
void RGB_SetBlue()
{
	h._mode = SB;
	
	h._CCR1 = 0;
	h._CCR2 = 0;
	h._CCR3 = RGB_MAX_REGISTER_VAL;
	
	_RGB_UpdateRegisters();
}
void RGB_SetBlack()
{
	h._mode = SN;
	
	h._CCR1 = 0;
	h._CCR2 = 0;
	h._CCR3 = 0;
	
	_RGB_UpdateRegisters();
}

void _RGB_UpdateRegisters()
{
	if (h._CCR1 > RGB_MAX_REGISTER_VAL) h._CCR1 = RGB_MAX_REGISTER_VAL;
	if (h._CCR2 > RGB_MAX_REGISTER_VAL) h._CCR2 = RGB_MAX_REGISTER_VAL;
	if (h._CCR3 > RGB_MAX_REGISTER_VAL) h._CCR3 = RGB_MAX_REGISTER_VAL;
	
	if (h._CCR1 < 0) h._CCR1 = 0;
	if (h._CCR2 < 0) h._CCR2 = 0;
	if (h._CCR3 < 0) h._CCR3 = 0;
	
	TIM2->CCR1 = h._CCR1;
	TIM2->CCR2 = h._CCR2;
	TIM2->CCR3 = h._CCR3;
	
	++h._state;
	if (h._state > RGB_MAX_REGISTER_VAL - 3) h._state = 0;
}

void RGB_Rainbow()
{
	if (h._mode != RR) h._state = 0;
	h._mode = RR;
	
	switch (h._state)
	{
	case 0: 
		{
			h._CCR1 = RGB_MAX_REGISTER_VAL;
			h._CCR2 = 0;
			h._CCR3 = 0;
			break;
		}
	case (int)(RGB_MAX_REGISTER_VAL / 3): 
		{
			h._CCR1 = 0;
			h._CCR2 = RGB_MAX_REGISTER_VAL;
			h._CCR3 = 0;
			break;
		}
	case (int)(RGB_MAX_REGISTER_VAL * 2 / 3): 
		{
			h._CCR1 = 0;
			h._CCR2 = 0;
			h._CCR3 = RGB_MAX_REGISTER_VAL;
			break;
		}
	default: 
		{
			if (h._state > 0 && (h._state < RGB_MAX_REGISTER_VAL / 3))
			{
				h._CCR1 -= 3;
				h._CCR2 += 3;
				h._CCR3 = 0;
			}
			else if ((h._state > RGB_MAX_REGISTER_VAL / 3) && (h._state < RGB_MAX_REGISTER_VAL * 2 / 3))
			{
				h._CCR1 = 0;
				h._CCR2 -= 3;
				h._CCR3 += 3;
			}
			else if ((h._state > RGB_MAX_REGISTER_VAL * 2 / 3) && (h._state < RGB_MAX_REGISTER_VAL))
			{
				h._CCR1 += 3;
				h._CCR2 = 0;
				h._CCR3 -= 3;
			}
		}
	}
	_RGB_UpdateRegisters();
	
}

void RGB_BlinkRed()
{
	if (h._mode != BR) h._state = 0;
	h._mode = BR;
	
	h._CCR2 = 0;
	h._CCR3 = 0;
	
	if (h._state == 0)
	{
		h._CCR1 = 0;	
	}
	else if (h._state < RGB_MAX_REGISTER_VAL / 2)
	{
		h._CCR1 += 2;
	}
	else
	{
		h._CCR1 -= 2;
	}
	
	_RGB_UpdateRegisters();
}
void RGB_BlinkGreen()
{
	if (h._mode != BG) h._state = 0;
	h._mode = BG;
	
	h._CCR1 = 0;
	h._CCR3 = 0;
	
	if (h._state == 0)
	{
		h._CCR2 = 0;	
	}
	else if (h._state < RGB_MAX_REGISTER_VAL / 2)
	{
		h._CCR2 += 2;
	}
	else
	{
		h._CCR2 -= 2;
	}
	
	_RGB_UpdateRegisters();
}
void RGB_BlinkBlue()
{
	if (h._mode != BB) h._state = 0;
	h._mode = BB;
	
	h._CCR1 = 0;
	h._CCR2 = 0;
	
	if (h._state == 0)
	{
		h._CCR3 = 0;	
	}
	else if (h._state < RGB_MAX_REGISTER_VAL / 2)
	{
		h._CCR3 += 2;
	}
	else
	{
		h._CCR3 -= 2;
	}
	
	_RGB_UpdateRegisters();
}
void RGB_BlinkWhite()
{
	if (h._mode != BW) h._state = 0;
	h._mode = BW;
	
	if (h._state == 0)
	{
		h._CCR1 = 0;	
		h._CCR2 = 0;	
		h._CCR3 = 0;	
	}
	else if (h._state < RGB_MAX_REGISTER_VAL / 2)
	{
		h._CCR1 += 2;
		h._CCR2 += 2;
		h._CCR3 += 2;
	}
	else
	{
		h._CCR1 -= 2;
		h._CCR2 -= 2;
		h._CCR3 -= 2;
	}
	
	_RGB_UpdateRegisters();
}

void _RGB_Test(uint8_t choice)
{
	switch (choice)
	{
	case 0: RGB_SetWhite(); break;
	case 1: RGB_SetBlack(); break;
	case 2: RGB_SetRed(); break;
	case 3: RGB_SetGreen(); break;
	case 4: RGB_SetBlue(); break;
	case 5: RGB_BlinkRed(); break;
	case 6: RGB_BlinkGreen(); break;
	case 7: RGB_BlinkBlue(); break;
	case 8: RGB_BlinkWhite(); break;
	case 9: RGB_Rainbow(); break;
	}
}