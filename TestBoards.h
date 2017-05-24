#pragma once
#include "Board.h"
class TestsBoards {
public:
    static void initTests();
    static Board getBoard(int index);
private:
    static std::vector<Board> m_testBoards;
};