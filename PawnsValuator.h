#pragma once
#include "Board.h"
#include "Valuator.h"


namespace Valuation
{
class PawnsValuator: public Valuator {
public:
    static int getPositionalValue(const Board & chessboard, int myColor, GameState phase);
	
private:
    static int getOneSidePositionalValue(const Board & chessboard, int color, GameState phase);
    static int countValueForProximityToCenter(int pos, GameState phase);
    static int countValueForComingPawns(const Board & chessboard, int color, int pos, GameState phase);
    static int countValueForIzolation(const Board & chessboard, int color, int pos);
    static int countValueForMirroring(const Board & chessboard, int color, int pos);
    static bool izolatedPawn(const Board & chessboard, int color, int pos);
	static bool mirroredPawn(const Board & chessboard, int color, int pos);   
};
}