#pragma once
#include "Valuator.h"

namespace Valuation {

class KingsValuator: public Valuator{
public:
    static int getPositionalValue(const Board & chessboard, int color, GameState phase);

private:
    static int getOneSidePositionalValue(const Board & chessboard, int color, GameState phase);
    static int getValueForProximityToCenterOnEnding(GameState phase, int myKing, int color, const Board &chessboard);
    static int getValueForSafetyKing(int kingPos, int color, GameState phase);
    static int getValueForWallOfPawns(const Board & chessboard, int color, int kingPos);
    static std::vector<int> getKingSafetyTab();
};
}