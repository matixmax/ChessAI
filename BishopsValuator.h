#pragma once
#include "Valuator.h"

namespace Valuation {

class BishopsValuator : public Valuator {
public:
    static int getPositionalValue(const Board & chessboard, int color, GameState phase);

private:
    static int getOneSidePositionalValue(const Board & chessboard, int color, GameState phase);
    static int getValueForMobilityOfBishops(int my_pos);
    static int getValueForFiguresOnDiagonals(const Board & chessboard, int my_pos, int myColor);
    static std::vector<int> getFiguresOnDiagonals(int my_pos, const Board &chessboard);
    static std::vector<int> getBishopMovementsTab();
    static std::vector<int> getWhiteFigPointsTab();
    static std::vector<int> getBlackFigPointsTab();
};
}