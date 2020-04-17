#include "SP_RGB.h"

#define MAX_REG_VAL 999

uint32_t _CCR1; /* RED */
uint32_t _CCR2; /* GREEN */
uint32_t _CCR3; /* BLUE */

uint16_t _state;
RGB_Mode R_Mode;

void RGB_Init(void) {
	_CCR1 = 0;
	_CCR2 = 0;
	_CCR3 = 0;
	_state = 0;
	R_Mode = RGB_Rainbow;
}

void _RGB_UpdateRegisters(void) {
	if (_CCR1 > MAX_REG_VAL)
		_CCR1 = MAX_REG_VAL;
	if (_CCR2 > MAX_REG_VAL)
		_CCR2 = MAX_REG_VAL;
	if (_CCR3 > MAX_REG_VAL)
		_CCR3 = MAX_REG_VAL;

	if (_CCR1 < 0)
		_CCR1 = 0;
	if (_CCR2 < 0)
		_CCR2 = 0;
	if (_CCR3 < 0)
		_CCR3 = 0;

	TIM2->CCR1 = _CCR1;
	TIM2->CCR2 = _CCR2;
	TIM2->CCR3 = _CCR3;

	++_state;
	if (_state > MAX_REG_VAL - 3)
		_state = 0;
}

void _RGB_SetWhite(void) {
	R_Mode = RGB_White;

	_CCR1 = MAX_REG_VAL;
	_CCR2 = MAX_REG_VAL;
	_CCR3 = MAX_REG_VAL;

	_RGB_UpdateRegisters();
}

void _RGB_SetRed(void) {
	R_Mode = RGB_Red;

	_CCR1 = MAX_REG_VAL;
	_CCR2 = 0;
	_CCR3 = 0;

	_RGB_UpdateRegisters();
}

void _RGB_SetGreen(void) {
	R_Mode = RGB_Green;

	_CCR1 = 0;
	_CCR2 = MAX_REG_VAL;
	_CCR3 = 0;

	_RGB_UpdateRegisters();
}

void _RGB_SetBlue(void) {
	R_Mode = RGB_Blue;

	_CCR1 = 0;
	_CCR2 = 0;
	_CCR3 = MAX_REG_VAL;

	_RGB_UpdateRegisters();
}

void _RGB_SetBlack(void) {
	R_Mode = RGB_Disabled;

	_CCR1 = 0;
	_CCR2 = 0;
	_CCR3 = 0;

	_RGB_UpdateRegisters();
}

void _RGB_Rainbow(void) {
	if (R_Mode != RGB_Rainbow)
		_state = 0;
	R_Mode = RGB_Rainbow;

	switch (_state) {
	case 0: {
		_CCR1 = MAX_REG_VAL;
		_CCR2 = 0;
		_CCR3 = 0;
		break;
	}
	case (int) (MAX_REG_VAL / 3): {
		_CCR1 = 0;
		_CCR2 = MAX_REG_VAL;
		_CCR3 = 0;
		break;
	}
	case (int) (MAX_REG_VAL * 2 / 3): {
		_CCR1 = 0;
		_CCR2 = 0;
		_CCR3 = MAX_REG_VAL;
		break;
	}
	default: {
		if (_state > 0 && (_state < MAX_REG_VAL / 3)) {
			_CCR1 -= 3;
			_CCR2 += 3;
			_CCR3 = 0;
		} else if ((_state > MAX_REG_VAL / 3)
				&& (_state < MAX_REG_VAL * 2 / 3)) {
			_CCR1 = 0;
			_CCR2 -= 3;
			_CCR3 += 3;
		} else if ((_state > MAX_REG_VAL * 2 / 3) && (_state < MAX_REG_VAL)) {
			_CCR1 += 3;
			_CCR2 = 0;
			_CCR3 -= 3;
		}
	}
	}
	_RGB_UpdateRegisters();
}

void _RGB_BlinkRed(void) {
	if (R_Mode != RGB_BlinkRed)
		_state = 0;
	R_Mode = RGB_BlinkRed;

	_CCR2 = 0;
	_CCR3 = 0;

	if (_state == 0) {
		_CCR1 = 0;
	} else if (_state < MAX_REG_VAL / 2) {
		_CCR1 += 2;
	} else {
		_CCR1 -= 2;
	}

	_RGB_UpdateRegisters();
}

void _RGB_BlinkGreen(void) {
	if (R_Mode != RGB_BlinkGreen)
		_state = 0;
	R_Mode = RGB_BlinkGreen;

	_CCR1 = 0;
	_CCR3 = 0;

	if (_state == 0) {
		_CCR2 = 0;
	} else if (_state < MAX_REG_VAL / 2) {
		_CCR2 += 2;
	} else {
		_CCR2 -= 2;
	}

	_RGB_UpdateRegisters();
}

void _RGB_BlinkBlue(void) {
	if (R_Mode != RGB_BlinkBlue)
		_state = 0;
	R_Mode = RGB_BlinkBlue;

	_CCR1 = 0;
	_CCR2 = 0;

	if (_state == 0) {
		_CCR3 = 0;
	} else if (_state < MAX_REG_VAL / 2) {
		_CCR3 += 2;
	} else {
		_CCR3 -= 2;
	}

	_RGB_UpdateRegisters();
}

void _RGB_BlinkWhite(void) {
	if (R_Mode != RGB_BlinkWhite)
		_state = 0;
	R_Mode = RGB_BlinkWhite;

	if (_state == 0) {
		_CCR1 = 0;
		_CCR2 = 0;
		_CCR3 = 0;
	} else if (_state < MAX_REG_VAL / 2) {
		_CCR1 += 2;
		_CCR2 += 2;
		_CCR3 += 2;
	} else {
		_CCR1 -= 2;
		_CCR2 -= 2;
		_CCR3 -= 2;
	}

	_RGB_UpdateRegisters();
}

void RGB_Update() {
	switch (R_Mode) {
	case RGB_Disabled:
		_RGB_SetBlack();
		break;
	case RGB_White:
		_RGB_SetWhite();
		break;
	case RGB_Red:
		_RGB_SetRed();
		break;
	case RGB_Green:
		_RGB_SetGreen();
		break;
	case RGB_Blue:
		_RGB_SetBlue();
		break;
	case RGB_BlinkRed:
		_RGB_BlinkRed();
		break;
	case RGB_BlinkGreen:
		_RGB_BlinkGreen();
		break;
	case RGB_BlinkBlue:
		_RGB_BlinkBlue();
		break;
	case RGB_BlinkWhite:
		_RGB_BlinkWhite();
		break;
	case RGB_Rainbow:
		_RGB_Rainbow();
		break;
	}
}
