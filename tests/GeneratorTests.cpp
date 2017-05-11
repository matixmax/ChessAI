#include "gtest/gtest.h"
#include "../Board.h"
#include "../Generator.h"
#include "../BoardWizard.h"

using namespace BoardWizard;
using namespace std;

TEST(generator_tests, validateMovementTest)
{
    Board generatedInitialBoad = Wizard::create({
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Emp, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Pb5, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Pw1, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Pw2, Pw3, Pw4, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr });
    Generator generator(generatedInitialBoad);
    bool attacked = false;

    EXPECT_TRUE( generator.validateMovement(9, 8, attacked) );
    EXPECT_FALSE( attacked );
    EXPECT_TRUE( generator.validateMovement(generatedInitialBoad, 49, -24, attacked, 3) );
    EXPECT_TRUE( generator.validateMovement(generatedInitialBoad, 5, 35, attacked, 5) );
    EXPECT_TRUE( attacked );
}

TEST(generator_tests, validateMovementTest2)
{
    Board generatedInitialBoad = Wizard::create({
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Emp, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Pb5, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Pw1, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Pw2, Pw3, Pw4, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr });
    Generator generator(generatedInitialBoad);
    bool attacked = false;
    vector<int> moves = { 1,9,8,7,-1,-7,-8,-9 };
    int testSize = 100;
    for ( int i = 0; i < testSize; i++) {
        int currPos = rand() % BOARD_SIZE;
        int distance = (rand() % 8 + 1);
        int move = moves[rand() % moves.size()] * distance;
        bool attacked = false, attacked2 = false;
        if (generatedInitialBoad.board[currPos] != E) {
            generator.m_currentMoveDistance[generatedInitialBoad.colors[currPos]] = distance;
        }
        ASSERT_EQ(  generator.validateMovement(currPos, move, attacked), 
                    generator.validateMovement(generatedInitialBoad, currPos, move, attacked2, distance));
        ASSERT_EQ( attacked, attacked2 );
    }
}