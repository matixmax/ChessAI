#pragma once
#include "Board.h"
#include <map>

class BoardRememberer{
public:
    static BoardRememberer& i();
    void addBoard(Board board);
	bool moveIsDoubleSet(Board board);

private:
    bool isRemembered(Board board);

    std::map<int, unsigned> m_memory;
};
