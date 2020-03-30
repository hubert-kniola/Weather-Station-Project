#include "SP_RGB.h"

#define RGB_MAX_REGISTER_VAL 999

uint32_t _CCR1; /* RED */
uint32_t _CCR2; /* GREEN */
uint32_t _CCR3; /* BLUE */
	
uint16_t _state;
RGB_Mode _mode;

void RGB_Init(void)
{
	_CCR1 = 0;
	_CCR2 = 0;
	_CCR3 = 0;
	_state = 0;
	_mode = NONE;
}
void RGB_SetWhite(void)
{
	_mode = White;
	
	_CCR1 = RGB_MAX_REGISTER_VAL;
	_CCR2 = RGB_MAX_REGISTER_VAL;
	_CCR3 = RGB_MAX_REGISTER_VAL;
	
	_RGB_UpdateRegisters();
}
void RGB_SetRed(void)
{
	_mode = Red;
	
	_CCR1 = RGB_MAX_REGISTER_VAL;
	_CCR2 = 0;
	_CCR3 = 0;
	
	_RGB_UpdateRegisters();
}
void RGB_SetGreen(void)
{
	_mode = Green;
	
	_CCR1 = 0;
	_CCR2 = RGB_MAX_REGISTER_VAL;
	_CCR3 = 0;
	
	_RGB_UpdateRegisters();
}
void RGB_SetBlue(void)
{
	_mode = Blue;
	
	_CCR1 = 0;
	_CCR2 = 0;
	_CCR3 = RGB_MAX_REGISTER_VAL;
	
	_RGB_UpdateRegisters();
}
void RGB_SetBlack(void)
{
	_mode = Disabled;
	
	_CCR1 = 0;
	_CCR2 = 0;
	_CCR3 = 0;
	
	_RGB_UpdateRegisters();
}

void _RGB_UpdateRegisters(void)
{
	if (_CCR1 > RGB_MAX_REGISTER_VAL) _CCR1 = RGB_MAX_REGISTER_VAL;
	if (_CCR2 > RGB_MAX_REGISTER_VAL) _CCR2 = RGB_MAX_REGISTER_VAL;
	if (_CCR3 > RGB_MAX_REGISTER_VAL) _CCR3 = RGB_MAX_REGISTER_VAL;
	
	if (_CCR1 < 0) _CCR1 = 0;
	if (_CCR2 < 0) _CCR2 = 0;
	if (_CCR3 < 0) _CCR3 = 0;
	
	TIM2->CCR1 = _CCR1;
	TIM2->CCR2 = _CCR2;
	TIM2->CCR3 = _CCR3;
	
	++_state;
	if (_state > RGB_MAX_REGISTER_VAL - 3) _state = 0;
}

void RGB_Rainbow(void)
{
	if (_mode != Rainbow) _state = 0;
	_mode = Rainbow;
	
	switch (_state)
	{
	case 0: 
		{
			_CCR1 = RGB_MAX_REGISTER_VAL;
			_CCR2 = 0;
			_CCR3 = 0;
			break;
		}
	case (int)(RGB_MAX_REGISTER_VAL / 3): 
		{
			_CCR1 = 0;
			_CCR2 = RGB_MAX_REGISTER_VAL;
			_CCR3 = 0;
			break;
		}
	case (int)(RGB_MAX_REGISTER_VAL * 2 / 3): 
		{
			_CCR1 = 0;
			_CCR2 = 0;
			_CCR3 = RGB_MAX_REGISTER_VAL;
			break;
		}
	default: 
		{
			if (_state > 0 && (_state < RGB_MAX_REGISTER_VAL / 3))
			{
				_CCR1 -= 3;
				_CCR2 += 3;
				_CCR3 = 0;
			}
			else if ((_state > RGB_MAX_REGISTER_VAL / 3) && (_state < RGB_MAX_REGISTER_VAL * 2 / 3))
			{
				_CCR1 = 0;
				_CCR2 -= 3;
				_CCR3 += 3;
			}
			else if ((_state > RGB_MAX_REGISTER_VAL * 2 / 3) && (_state < RGB_MAX_REGISTER_VAL))
			{
				_CCR1 += 3;
				_CCR2 = 0;
				_CCR3 -= 3;
			}
		}
	}
	_RGB_UpdateRegisters();
}

void RGB_BlinkRed(void)
{
	if (_mode != BlinkRed) _state = 0;
	_mode = BlinkRed;
	
	_CCR2 = 0;
	_CCR3 = 0;
	
	if (_state == 0)
	{
		_CCR1 = 0;	
	}
	else if (_state < RGB_MAX_REGISTER_VAL / 2)
	{
		_CCR1 += 2;
	}
	else
	{
		_CCR1 -= 2;
	}
	
	_RGB_UpdateRegisters();
}
void RGB_BlinkGreen(void)
{
	if (_mode != BlinkGreen) _state = 0;
	_mode = BlinkGreen;
	
	_CCR1 = 0;
	_CCR3 = 0;
	
	if (_state == 0)
	{
		_CCR2 = 0;	
	}
	else if (_state < RGB_MAX_REGISTER_VAL / 2)
	{
		_CCR2 += 2;
	}
	else
	{
		_CCR2 -= 2;
	}
	
	_RGB_UpdateRegisters();
}
void RGB_BlinkBlue(void)
{
	if (_mode != BlinkBlue) _state = 0;
	_mode = BlinkBlue;
	
	_CCR1 = 0;
	_CCR2 = 0;
	
	if (_state == 0)
	{
		_CCR3 = 0;	
	}
	else if (_state < RGB_MAX_REGISTER_VAL / 2)
	{
		_CCR3 += 2;
	}
	else
	{
		_CCR3 -= 2;
	}
	
	_RGB_UpdateRegisters();
}
void RGB_BlinkWhite(void)
{
	if (_mode != BlinkWhite) _state = 0;
	_mode = BlinkWhite;
	
	if (_state == 0)
	{
		_CCR1 = 0;	
		_CCR2 = 0;	
		_CCR3 = 0;	
	}
	else if (_state < RGB_MAX_REGISTER_VAL / 2)
	{
		_CCR1 += 2;
		_CCR2 += 2;
		_CCR3 += 2;
	}
	else
	{
		_CCR1 -= 2;
		_CCR2 -= 2;
		_CCR3 -= 2;
	}
	
	_RGB_UpdateRegisters();
}

void _RGB_Test(uint8_t choice)
{
	switch (choice)
	{
	case 0: RGB_SetBlack(); break;
	case 1: RGB_SetWhite(); break;
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