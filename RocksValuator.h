#pragma once
#include "Board.h"
#include "Valuator.h"

namespace Valuation
{
class RocksValuator : public Valuator {
public:
    static int getPositionalValue(const Board & chessboard, int color, GameState phase);
    
private:
    static int getOneSidePositionalValue(const Board & chessboard, int color, GameState phase);
    static int getValueForRookOnSevenLine(const Board& chessboard, int rook_pos, int color);
    static int getValueForDistanceToEnemyKing(const Board &chessboard, int color, GameState phase, int rookPos);
    static int getValueForRookInOpenLine(const Board & chessboard, int rooks_pos, int rooks_color);
    static int getValueForConnectedRooksAndMobility(const Board & chessboard, int rooks_pos, int rooks_color);
};
}