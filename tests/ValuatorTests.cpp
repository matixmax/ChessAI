#include "gtest/gtest.h"
#include "../Board.h"
#include "../BoardWizard.h"
#include "../Valuator.h"
#include "../PawnsValuator.h"
#include "../RocksValuator.h"
#include "../QueensValuator.h"
#include "../KingsValuator.h"

using namespace Valuation;
using namespace BoardWizard;
using namespace std;

TEST(valuation_tests, countMaterialTest)
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
    vector<int>figValues = {    20000,  //king
                                950,    //queen
                                500,    //rook
                                290,    //knight
                                310,    //bishop
                                100,    //black pawn
                                100 };  //white pawn

    Board commonBoard = Wizard::create(board);
    Valuator valuator;
    valuator.m_figValues = figValues;

    const int sumPawns = 10, sumRocks = 2, sumBish = 2, sumKnights = 2, sumQeens = 1;
    //into material isn't counted kings values because kings is undestroyable;
    int maxMaterial = sumPawns * figValues[Pc] 
                    + sumRocks * figValues[W] 
                    + sumBish * figValues[G] 
                    + sumKnights * figValues[S] 
                    + sumQeens * figValues[H];

    EXPECT_GE(valuator.countMaterial(commonBoard).first, 0);
    EXPECT_LE(valuator.countMaterial(commonBoard).first, maxMaterial);
    EXPECT_GE(valuator.countMaterial(commonBoard).second, 0);
    EXPECT_LE(valuator.countMaterial(commonBoard).second, maxMaterial);
}

TEST(valuation_tests, strangeBoardTest)
{
    array<Figure, BOARD_SIZE> board = {
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Pb5, Emp, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Pb6, Emp, Emp,
        Emp, Emp, Pw3, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Pw4, Emp, Emp, Emp, Emp,
        Pw1, Pw2, Emp, Emp, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr };
    vector<int>figValues = { 20000,  //king
        950,    //queen
        500,    //rook
        290,    //knight
        310,    //bishop
        100,    //black pawn
        100 };  //white pawn

    Board commonBoard = Wizard::create(board);
    Valuator valuator;
    valuator.m_figValues = figValues;

    int result1 = valuator.materialValuation(commonBoard, BLACK);
    int result2 = PawnsValuator::getPositionalValue(commonBoard, BLACK, DEBUT);
    int result3 = valuator.knightsPositionalValue(commonBoard, BLACK);
    int result4 = valuator.bishopsPositionalValue(commonBoard, BLACK, DEBUT);
    int result5 = RocksValuator::getPositionalValue(commonBoard, BLACK, DEBUT);
    int result6 = QueenValuator::getPositionalValue(commonBoard, BLACK, DEBUT);
    int result7 = KingsValuator::getPositionalValue(commonBoard, BLACK, DEBUT);
}