#pragma once
#include <vector>
#include "BoardStates.h"
#include "Definitions.h"

struct Board {
    std::vector<int> board;
    std::vector<int> positions;
    std::vector<int> colors;
    BoardStates states;
    Board();
    Board(  std::vector<int> board, 
            std::vector<int> positions, 
            std::vector<int> colors, 
            BoardStates state = BoardStates());
    static Board createEmptyBoard();
};