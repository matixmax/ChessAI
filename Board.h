#pragma once
#include <vector>
#include "BoardStates.h"
#include "Definitions.h"

struct Board {
	std::vector<int8> board;
	std::vector<int8> positions;
	std::vector<int8> colors;
	BoardStates states;
	Board();
	Board(	std::vector<int8> board, 
			std::vector<int8> positions, 
			std::vector<int8> colors, 
			BoardStates state = BoardStates());
};