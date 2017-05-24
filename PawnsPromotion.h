#pragma once
#include "Board.h"

class PawnsPromotion {
public:
    static Board promoteAll(Board chessboard);
    static Board promotePawns(Board chessboard, int color);
private:
    static Board setFigureAfterPromotion(const Board &chessboard, int color, int pawnPos);
    static Board exchangePromotionFig(const Board &chessboard, int pawnPos, int promotePos);
};