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
        m_memory.emplace(std::make_pair(BoardWizard::Wizard::getHash(board), 1));
	m_memory.at(BoardWizard::Wizard::getHash(board))++;
}

bool BoardRememberer::isRemembered(Board board)
{
	auto it = m_memory.find(BoardWizard::Wizard::getHash(board));
    return it != m_memory.end();
}

bool BoardRememberer::moveIsDoubleSet(Board board)
{
	if (!isRemembered(board))
		return false;
	return m_memory.at(BoardWizard::Wizard::getHash(board)) >= 2;
}
