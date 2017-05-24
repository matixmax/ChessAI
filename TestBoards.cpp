#include "testBoards.h"
#include <array>
#include "BoardWizard.h"
#include <string>

using namespace BoardWizard;
using namespace std;

void TestsBoards::initTests()
{
    array<Figure, BOARD_SIZE> board = {
        Emp, Emp, Emp, Emp, Kgb, Emp, Emp, Emp,
        Emp, Pw2, Emp, Emp, Pb5, Emp, Pw7, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Pb8,
        Emp, Emp, Pb3, Emp, Pw5, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Kgw, Emp, Emp, Emp };

    Board commonBoard = Wizard::create(board);
    m_testBoards.push_back(commonBoard);
}

Board TestsBoards::getBoard(int index)
{
    if (index > m_testBoards.size())
        throw runtime_error("don't have test board with " + to_string(index) + " index");
    return m_testBoards[index];
}

vector<Board> TestsBoards::m_testBoards = vector<Board>();
