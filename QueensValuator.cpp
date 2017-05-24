#include "QueensValuator.h"
#include "FigInfo.h"

namespace Valuation{

int QueenValuator::getPositionalValue(const Board & chessboard, int color, GameState phase)
{
    int value = 0;

    value += getOneSidePositionalValue(chessboard, color, phase);
    value -= getOneSidePositionalValue(chessboard, FigInfo::not(color), phase);
    
    return value;
}

int QueenValuator::getOneSidePositionalValue(const Board & chessboard, int color, GameState phase)
{
    int value = 0;
    int queenPosition = chessboard.positions[FigInfo::getPosIndex(H, color)];

    value += getValueForQueenProximityToCenter(queenPosition, phase);
    value += getValueForQueenProximityToEnemyKing(chessboard, color, phase, queenPosition);
    
    return value;
}

int QueenValuator::getValueForQueenProximityToEnemyKing(const Board &chessboard, int color, GameState phase, int queenPos)
{
    int enemyKing = chessboard.positions[FigInfo::getPosIndex(K, FigInfo::not(color))];
    if (phase != DEBUT) {
        return 3 * proximityOfFields(queenPos, enemyKing);
    }
    return 0;
}

int QueenValuator::getValueForQueenProximityToCenter(int queenPos, GameState phase)
{
    int factor = 0;
    switch (phase) {
    case DEBUT:
        factor = -2;
        break;
    case MID_GAME:
        factor = 0;
        break;
    case EARLY_ENDING:
        factor = 2;
        break;
    case ENDING:
        factor = 4;
        break;
    }
    return factor * proximityToTheCenter(queenPos);
}

}

