#include "testBoards.h"
#include <array>
#include "BoardWizard.h"
#include <string>

using namespace BoardWizard;
using namespace std;

void TestsBoards::initTests()
{
	//0
    array<Figure, BOARD_SIZE> board = {
        Rbl, Emp, Emp, Emp, Kgb, Emp, Emp, Rbr,
        Emp, Pw2, Emp, Emp, Pb5, Emp, Pw7, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Pb8,
        Emp, Emp, Pb3, Emp, Pw5, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Kgw, Emp, Emp, Emp };

    Board commonBoard = Wizard::create(board);
    m_testBoards.push_back(commonBoard);

	//1
    board = {
        Rbl, Emp, Emp, Emp, Kgb, Emp, Emp, Rbr,
        Emp, Emp, Emp, Emp, Emp, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Kgw, Emp, Emp, Emp };

    commonBoard = Wizard::create(board);
    m_testBoards.push_back(commonBoard);

	//2
	board = {
		Emp, Emp, Emp, Emp, Emp, Emp, Kgb, Emp,
		Emp, Rbl, Emp, Emp, Emp, Emp, Emp, Emp,
		Pb1, Emp, Emp, Bbl, Emp, Emp, Emp, Pb2,
		Pw1, Rbr, Pb3, Kbl, Emp, Emp, Pb4, Emp,
		Emp, Pw2, Pb5, Emp, Emp, Emp, Pw3, Emp,
		Emp, Emp, Pw4, Emp, Emp, Emp, Emp, Emp,
		Emp, Emp, Emp, Emp, Emp, Emp, Qnw, Emp,
		Emp, Emp, Emp, Qnb, Emp, Kgw, Emp, Emp };

	commonBoard = Wizard::create(board);
	m_testBoards.push_back(commonBoard);
	
	//3
	board = { 
		Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
		Emp, Emp, Emp, Emp, Emp, Emp, Pb1, Emp,
		Emp, Emp, Emp, Emp, Emp, Emp, Pw1, Pb2,
		Emp, Emp, Emp, Emp, Emp, Kgb, Emp, Pw2,
		Emp, Pb3, Emp, Emp, Emp, Emp, Emp, Emp,
		Pb4, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
		Rwl, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
		Emp, Emp, Rbl, Emp, Emp, Emp, Kgw, Emp };

	commonBoard = Wizard::create(board);
	m_testBoards.push_back(commonBoard);
}

Board TestsBoards::getBoard(int index)
{
    if (index > m_testBoards.size())
        throw runtime_error("don't have test board with " + to_string(index) + " index");
    return m_testBoards[index];
}

vector<Board> TestsBoards::m_testBoards = vector<Board>();
