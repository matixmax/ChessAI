#pragma once
#include "Valuator.h"

namespace Valuation {

class KnightsValuator : public Valuator {
public:
    static int getPositionalValue(const Board & chessboard, int color);

private:
    static int getOneSidePositionalValue(const Board & chessboard, int color);
};
}