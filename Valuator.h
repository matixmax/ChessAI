#pragma once

#include "Definitions.h"
#include "Board.h"
#include "properties.h"

namespace Valuation {

enum GameState {
    DEBUT,
    MID_GAME,
    EARLY_ENDING,
    ENDING,
    MATTING,
};

class Valuator {
public:
    static Valuator& i();
    GameState getGamePhase(const Board & chessboard);
    int materialValuation(const Board & chessboard, int color);
    int mattingPositionalValue(const Board & chessboard, int color);
    std::pair<int, int> countMaterial(const Board & chessboard);

protected:
    Valuator();

    static int proximityToTheCenter(int pos);
    static int proximityOfFields(int pos1, int pos2);
    bool matCondition(const Board &chessboard, int color);
    int getFiguresInStartPos(const Board & chessboard);
    int countSumOfPawns(const Board & chessboard, int color);

    std::vector<int> m_figValues;
};

}