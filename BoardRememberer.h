#pragma once
#include "Board.h"

class BoardRememberer{
public:
    static BoardRememberer& i();
    void addBoard(Board board);
    bool isRemembered(Board board);
private:
    std::vector<int> m_memory;
};
