#include "KnightsValuator.h"
#include "FigInfo.h"

namespace Valuation{

int Valuation::KnightsValuator::getPositionalValue(const Board & chessboard, int color)
{
    int value = 0;
    value += getOneSidePositionalValue(chessboard, color);
    value -= getOneSidePositionalValue(chessboard, FigInfo::not(color));
    return Properies::knightsPosFact[color] * value;
}

int KnightsValuator::getOneSidePositionalValue(const Board & chessboard, int color)
{
    int factor = 4;
    int valuate = 0;
    valuate += factor * proximityToTheCenter(chessboard.positions[FigInfo::getPosIndex(S, color, 0)]);
    valuate += factor * proximityToTheCenter(chessboard.positions[FigInfo::getPosIndex(S, color, 1)]);
    return valuate;
}

}