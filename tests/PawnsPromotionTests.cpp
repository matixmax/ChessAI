#include "gtest/gtest.h"
#include "../Board.h"
#include "../BoardWizard.h"
#include "../PawnsPromotion.h"

using namespace BoardWizard;
using namespace std;

TEST(pawnsPromotion_tests, basicTest)
{
    array<Figure, BOARD_SIZE> board = {
        Pw1, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Emp, Emp, Pb3, Emp, Pb5, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Pw2, Pw3, Emp, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Pb4, Kgw, Bwr, Kwr, Rwr };

    Board commonBoard = Wizard::create(board);
    Board afterPromotion = PawnsPromotion::promoteAll(commonBoard);

    EXPECT_EQ(afterPromotion.board[0], H);
    EXPECT_EQ(afterPromotion.board[59], W);
}