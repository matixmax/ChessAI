#include "PawnsPromotion.h"
#include "FigInfo.h"

using namespace std;


Board PawnsPromotion::promoteAll(Board chessboard)
{
    chessboard = promotePawns(chessboard, WHITE);
    chessboard = promotePawns(chessboard, BLACK);
    return chessboard;
}

Board PawnsPromotion::promotePawns(Board chessboard, int color)
{
    vector<pair<int, int>> rowBoards = { { 56, 63 },{ 0, 7 } };
    for (int pawnIdx = 0; pawnIdx < 8; pawnIdx++) {
        int pawnPosIdx = FigInfo::getPosIndex(P, color, pawnIdx);
        int pawnPos = chessboard.positions[pawnPosIdx];
        if (rowBoards[color].first <= pawnPos && rowBoards[color].second >= pawnPos) {
            chessboard = setFigureAfterPromotion(chessboard, color, pawnPosIdx);
        }
    }
    return chessboard;
}

Board PawnsPromotion::setFigureAfterPromotion(const Board &chessboard, int color, int pawnPosIdx)
{
    const int numberOfHighFig = 8;
    const int colorIdxSwitch = (NUMBER_OF_POSITIONS / 2)*color;
    for (int posIdx = colorIdxSwitch * color; posIdx < colorIdxSwitch + numberOfHighFig; posIdx++) {
        if (chessboard.positions[posIdx] == DESTROYED) {
            return exchangePromotionFig(chessboard, pawnPosIdx, posIdx);
        }
    }
    return chessboard;
}

Board PawnsPromotion::exchangePromotionFig(const Board &chessboard, int pawnPosIdx, int promotePosIdx)
{
    assert(chessboard.positions[promotePosIdx] == DESTROYED && chessboard.positions[pawnPosIdx] != DESTROYED);
    Board result = chessboard;
    result.positions[promotePosIdx] = result.positions[pawnPosIdx];
    result.positions[pawnPosIdx] = DESTROYED;
    result.board[result.positions[promotePosIdx]] = FigInfo::getFigNumber(promotePosIdx);
    return result;
}

