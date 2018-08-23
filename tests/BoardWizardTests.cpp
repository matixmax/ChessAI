#include "stdafx.h"
#include "Board.h"
#include "BoardWizard.h"

using namespace BoardWizard;

TEST(boardWizad_tests, initialBoardTest)
{
    Board initialBoard;
    initialBoard.board =
    { W, S, G, H, K, G, S, W,
        Pc,Pc,Pc,Pc,Pc,Pc,Pc,Pc,
        E, E, E, E, E, E, E, E,
        E, E, E, E, E, E, E, E,
        E, E, E, E, E, E, E, E,
        E, E, E, E, E, E, E, E,
        Pb,Pb,Pb,Pb,Pb,Pb,Pb,Pb,
        W, S, G, H, K, G, S, W };
    initialBoard.colors =
    { BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
        E,        E,       E,      E,     E,        E,       E,      E,
        E,        E,       E,      E,     E,        E,       E,      E,
        E,        E,       E,      E,     E,        E,       E,      E,
        E,        E,       E,      E,     E,        E,       E,      E,
        WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
        WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE };
    initialBoard.positions =
    { 4,  3,  0,  7,  1,  6,  2,  5, // position K H W W S S G G
        8,  9,  10, 11, 12, 13, 14, 15,//            PbPbPbPbPbPbPbPb
        60, 59, 56, 63, 57, 62, 58, 61,//            K H W W S S G G
        48, 49, 50, 51, 52, 53, 54, 55 };//            PwPwPwPbwbPwPwPw

    Board generatedInitialBoad = Wizard::create({
        Rbl, Kbl, Bbl, Qnb, Kgb, Bbr, Kbr, Rbr,
        Pb1, Pb2, Pb3, Pb4, Pb5, Pb6, Pb7, Pb8,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Emp, Emp, Emp, Emp, Emp, Emp, Emp, Emp,
        Pw1, Pw2, Pw3, Pw4, Pw5, Pw6, Pw7, Pw8,
        Rwl, Kwl, Bwl, Qnw, Kgw, Bwr, Kwr, Rwr });

    for (int idx = 0; idx < BOARD_SIZE; idx++) {
        EXPECT_EQ(generatedInitialBoad.board[idx], initialBoard.board[idx]);
        EXPECT_EQ(generatedInitialBoad.colors[idx], initialBoard.colors[idx]);
    }
    for (int idx = 0; idx < NUMBER_OF_POSITIONS; idx++) {
        EXPECT_EQ(generatedInitialBoad.positions[idx], initialBoard.positions[idx]);
    }
}