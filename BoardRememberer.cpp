#include "BoardRememberer.h"
#include "BoardWizard.h"
#include <algorithm>

BoardRememberer& BoardRememberer::i()
{
    static BoardRememberer instance;
    return instance;
}

void BoardRememberer::addBoard(Board board)
{
    if (!isRemembered(board))
        m_memory.push_back(BoardWizard::Wizard::getHash(board));
}

bool BoardRememberer::isRemembered(Board board)
{
    auto it = std::find(m_memory.begin(), m_memory.end(), BoardWizard::Wizard::getHash(board));
    return it != m_memory.end();
}
