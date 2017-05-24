#pragma once
#include "Valuator.h"


namespace Valuation{

class QueenValuator : public Valuator {
public:
    static int getPositionalValue(const Board & chessboard, int color, GameState phase);

private:
    static int getOneSidePositionalValue(const Board & chessboard, int color, GameState phase);
    static int getValueForQueenProximityToCenter(int queenPos, GameState phase);
    static int getValueForQueenProximityToEnemyKing(const Board &chessboard, int color, GameState phase, int queenPos);
};
}