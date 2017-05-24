#include "KingsValuator.h"
#include "FigInfo.h"

using namespace std;

namespace Valuation{

int KingsValuator::getPositionalValue(const Board & chessboard, int color, GameState phase)
{
    int value = 0;
    value += getOneSidePositionalValue(chessboard, color, phase);
    value -= getOneSidePositionalValue(chessboard, FigInfo::not(color), phase);
    return value;
}

int KingsValuator::getOneSidePositionalValue(const Board & chessboard, int color, GameState phase)
{
    int myKing = chessboard.positions[FigInfo::getPosIndex(K, color)];
    if (myKing == DESTROYED)
        return 0;
    
    int value = 0;
    value += getValueForSafetyKing(myKing, color, phase);
    value += getValueForProximityToCenterOnEnding(phase, myKing, color, chessboard);
    value += getValueForWallOfPawns(chessboard, color, myKing);

    return value;
}

int KingsValuator::getValueForProximityToCenterOnEnding(GameState phase, int myKing, int color, const Board &chessboard)
{
    int factor;

    switch (phase) 
    {
    case EARLY_ENDING:
        factor = 3;
        break;
    case ENDING:
        factor = 8;
        break;
    default:
        factor = 0;
    }

    return factor * proximityToTheCenter(chessboard.positions[FigInfo::getPosIndex(K, color)]);
}

int KingsValuator::getValueForSafetyKing(int kingPos, int color, GameState phase) {
    
    if (phase != MID_GAME && phase != DEBUT)
        return 0;
    
    vector<int> king_safety = getKingSafetyTab();
    if (color == WHITE) {
        return (kingPos / ROW_SIZE < 4) ? -400 : king_safety[kingPos];
    }
    else {
        return (kingPos / ROW_SIZE >= 4) ? -400 : king_safety[kingPos];
    }
}

std::vector<int> KingsValuator::getKingSafetyTab()
{
    return{   15,   25,   10,   -5,  -10,   5,    25,   15,
              10,   15,  -40,  -40,  -40, -40,    15,   10,
            -100, -100, -100, -100, -100, -100, -100, -100,
            -200, -200, -200, -200, -200, -200, -200, -200,
            -200, -200, -200, -200, -200, -200, -200, -200,
            -100, -100, -100, -100, -100, -100, -100, -100,
              10,   15,  -40,  -40,  -40,  -40,   15,   10,
              15,   25,   10,   -5,  -10,    5,   25,   15 };
}

int KingsValuator::getValueForWallOfPawns(const Board & chessboard, int color, int kingPos) {
    int value = 0;
    int colorDirection = (color == WHITE) ? -1 : 1;
    int lastLine = (color == WHITE) ? 7 : 0;

    if (kingPos / ROW_SIZE == lastLine) {
        int leftPawnPos = kingPos + ROW_SIZE * colorDirection - 1;
        if (kingPos % ROW_SIZE != 0 && chessboard.board[leftPawnPos] == P + color)
            value += 15;

        int centerPawnPos = kingPos + ROW_SIZE * colorDirection;
        if(chessboard.board[centerPawnPos] == P + color)
            value += 15;

        int rightPawnPos = kingPos + ROW_SIZE * colorDirection + 1;
        if (kingPos % ROW_SIZE != 7 && chessboard.board[ rightPawnPos ] == P + color)
            value += 15;
    }

    if (value == 45)// if three positions in front of king is occupied by pawns 
        return 0;   // king have less opportunities to escape
    else
        return value;
}

}