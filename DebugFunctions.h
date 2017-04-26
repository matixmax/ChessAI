#pragma once
#include "Definitions.h"

std::vector<Board> GenerateMoves(const Board &position, int8 color);
void printValues(const Board &test_board, int8 color);
void posValuationTest();
void boardStatesAndAttackPosTest();