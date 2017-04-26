#pragma once
#include<vector>
#include "Definitions.h"
#include "Board.h"

class Debug {
public:
	std::vector<Board> GenerateMoves(const Board &position, int8 color);
	void printValues(const Board &test_board, int8 color);
	void posValuationTest();
	void boardStatesAndAttackPosTest();
};