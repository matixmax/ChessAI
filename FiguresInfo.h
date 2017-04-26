#pragma once
#include "Definitions.h"

class FigInfo {
public:
	int8 getPosIndex(int8 fig, int8 color, int8 number = 0);
	int8 getFigNumber(int8 pos_index, int8 color);
	int8 not(int8 color);
};