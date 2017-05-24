#include "gtest/gtest.h"
#include "../Board.h"
#include "../BoardWizard.h"
#include "../Valuator.h"
#include "../PawnsValuator.h"

using namespace Valuation;
using namespace BoardWizard;
using namespace std;

TEST(pawnsValuation_tests, initialValueTest)
{
    array<Figure, BOARD_SIZE> board = {
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Pb5, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Pw1, Pw2, Pw3, Pw4, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr };

    Board commonBoard = Wizard::create(board);
    int pawnsPosValue = PawnsValuator::getPositionalValue(commonBoard, WHITE, DEBUT);
    int blackPawnsPosValue = PawnsValuator::getOneSidePositionalValue(commonBoard, BLACK, DEBUT);
    int whitePawnsPosValue = PawnsValuator::getOneSidePositionalValue(commonBoard, WHITE, DEBUT);

    EXPECT_EQ(blackPawnsPosValue, whitePawnsPosValue);
    EXPECT_EQ(pawnsPosValue, 0);
}

TEST(pawnsValuation_tests, whiteBetterTest)
{
    array<Figure, BOARD_SIZE> board = {
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Pb5, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Pw3, Emp, Emp, Emp, Emp, Emp,
        Emp, Pw2, Emp, Pw4, Emp, Emp, Emp, Emp,
        Pw1, Emp, Emp, Emp, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr };

    Board commonBoard = Wizard::create(board);
    int pawnsPosValue = PawnsValuator::getPositionalValue(commonBoard, WHITE, DEBUT);
    int blackPawnsPosValue = PawnsValuator::getOneSidePositionalValue(commonBoard, BLACK, DEBUT);
    int whitePawnsPosValue = PawnsValuator::getOneSidePositionalValue(commonBoard, WHITE, DEBUT);

    EXPECT_TRUE(blackPawnsPosValue < whitePawnsPosValue);
}

TEST(pawnsValuation_tests, blackBetterOnEndingTest)
{
    array<Figure, BOARD_SIZE> board = {
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Emp, Emp, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Pw3, Pb4, Emp, Emp, Emp, Emp,
        Emp, Pw2, Emp, Pw4, Pb5, Pw6, Emp, Emp,
        Pw1, Emp, Emp, Emp, Pw5, Emp, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr };

    Board commonBoard = Wizard::create(board);
    int pawnsPosValue = PawnsValuator::getPositionalValue(commonBoard, WHITE, ENDING);
    int blackPawnsPosValue = PawnsValuator::getOneSidePositionalValue(commonBoard, BLACK, ENDING);
    int whitePawnsPosValue = PawnsValuator::getOneSidePositionalValue(commonBoard, WHITE, ENDING);

    EXPECT_TRUE(blackPawnsPosValue > whitePawnsPosValue);
}