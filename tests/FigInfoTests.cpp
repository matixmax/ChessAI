#include "gtest/gtest.h"
#include "../Board.h"
#include "../BoardWizard.h"
#include "../FigInfo.h"

using namespace BoardWizard;
using namespace std;

TEST(figInfo_tests, getPosIndexTest)
{
    array<Figure, BOARD_SIZE> board = {
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Emp, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Pb5, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Pw1, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Pw2, Pw3, Pw4, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr };
    Board generatedBoad = Wizard::create(board);

    int testSize = 100;
    for (int i = 0; i < testSize; i++) {
        int currPos = rand() % BOARD_SIZE;
        while (generatedBoad.board[currPos] == EMPTY) {
            currPos = rand() % BOARD_SIZE;
        }
        int fig = generatedBoad.board[currPos];
        int color = generatedBoad.colors[currPos];
        int number = Wizard::getFigureCommonData(board[currPos]).number;
        int posIdx = FigInfo::getPosIndex(fig, color, number);
        EXPECT_EQ(generatedBoad.positions[posIdx], currPos);
    }
}

TEST(figInfo_tests, getFigNumberTest)
{
    array<Figure, BOARD_SIZE> board = {
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Emp, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Pb5, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Pw1, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Pw2, Pw3, Pw4, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr };
    Board generatedBoad = Wizard::create(board);

    int testSize = 100;
    for (int i = 0; i < testSize; i++) {
        int posIdx = rand() % NUMBER_OF_POSITIONS;
        int currPos = generatedBoad.positions[posIdx];
        int fig = generatedBoad.board[currPos];
        int color = generatedBoad.colors[currPos];
        EXPECT_EQ(fig, FigInfo::getFigNumber(posIdx, color));
    }
}
