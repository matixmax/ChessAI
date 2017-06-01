#pragma once
#include<vector>
#include "Definitions.h"
#include "Board.h"

class Statistics {
public:
    static std::vector<Board> GenerateMoves(const Board &position, int color);
    static void printValues(const Board &test_board, int color);
    static void printValues(const Board &test_board, int color, std::ostream &output);
    static void posValuationTest();
    static void boardStatesAndAttackPosTest();
};