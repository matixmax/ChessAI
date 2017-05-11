#pragma once
#include "Board.h"
#include "Special.h"

extern Board g_baseBoard;
extern std::vector<Special*> g_specials;
extern std::vector<int8> *g_figMoves;

extern const int c_unknownPosition;