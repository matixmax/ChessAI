#include "BishopsValuator.h"
#include "FigInfo.h"

using namespace std;

namespace Valuation {


int BishopsValuator::getPositionalValue(const Board & chessboard, int color, GameState phase)
{
    int value = 0;
    value += getOneSidePositionalValue(chessboard, color, phase);
    value -= getOneSidePositionalValue(chessboard, FigInfo::not(color), phase);
    return value;
}

int BishopsValuator::getOneSidePositionalValue(const Board & chessboard, int color, GameState phase)
{
    if (phase != MID_GAME && phase != EARLY_ENDING)
        return 0;
    int factor = (phase == MID_GAME) ? 3 : 1;

    int value = 0;
    const int numberOfBishops = 2;
    for (int i = 0; i < numberOfBishops; i++) {
        int my_pos = chessboard.positions[FigInfo::getPosIndex(G, color, i)];
        if (my_pos == DESTROYED)
            continue;
        value += getValueForFiguresOnDiagonals(chessboard, my_pos, color);
        value += getValueForMobilityOfBishops(my_pos);
    }
    return factor * value;
}

int BishopsValuator::getValueForMobilityOfBishops(int my_pos)
{
    vector<int> bishop_movements = getBishopMovementsTab();

    return 7 * bishop_movements[my_pos];
}

int BishopsValuator::getValueForFiguresOnDiagonals(const Board & chessboard, int my_pos, int myColor)
{
    vector<int> myFigOnDiagPoints = (myColor == WHITE) ? getWhiteFigPointsTab() : getBlackFigPointsTab();
    vector<int> enemyFigOnDiagPoints = (myColor == WHITE) ? getBlackFigPointsTab() : getWhiteFigPointsTab();

    int value = 0;
    vector<int> figuresInDiagonals = getFiguresOnDiagonals(my_pos, chessboard);

    for (int j = 0; j < 4; j++) {
        if (figuresInDiagonals[j] != DESTROYED) {
            if (FigInfo::getColor(figuresInDiagonals[j]) == myColor)
                value += myFigOnDiagPoints[figuresInDiagonals[j]];
            else
                value += enemyFigOnDiagPoints[figuresInDiagonals[j]];
        }
    }
    return value;
}

vector<int> BishopsValuator::getFiguresOnDiagonals(int my_pos, const Board &chessboard)
{
    vector<int> figuresInDiagonals = { DESTROYED, DESTROYED, DESTROYED, DESTROYED };//diagonals in accordance with the clockwise

    int max_distance = 7 - my_pos % ROW_SIZE; // to the right
    for (int distance = 1; distance <= max_distance; distance++) {
        if (figuresInDiagonals[0] == DESTROYED && my_pos + (-7 * distance) > 0 && chessboard.board[my_pos + (-7 * distance)] != EMPTY)
            figuresInDiagonals[0] = FigInfo::getPosIndex(chessboard.board[my_pos + (-7 * distance)], chessboard.colors[my_pos + (-7 * distance)]);
        if (figuresInDiagonals[1] == DESTROYED && my_pos + (9 * distance) < BOARD_SIZE && chessboard.board[my_pos + (9 * distance)] != EMPTY)
            figuresInDiagonals[1] = FigInfo::getPosIndex(chessboard.board[my_pos + (9 * distance)], chessboard.colors[my_pos + (9 * distance)]);
    }
    max_distance = my_pos % ROW_SIZE; // to the left
    for (int distance = 1; distance <= max_distance; distance++) {
        if (figuresInDiagonals[2] == DESTROYED && my_pos + (7 * distance) < BOARD_SIZE && chessboard.board[my_pos + (7 * distance)] != EMPTY)
            figuresInDiagonals[2] = FigInfo::getPosIndex(chessboard.board[my_pos + (7 * distance)], chessboard.colors[my_pos + (7 * distance)]);
        if (figuresInDiagonals[3] == DESTROYED && my_pos + (-9 * distance) > 0 && chessboard.board[my_pos + (-9 * distance)] != EMPTY)
            figuresInDiagonals[3] = FigInfo::getPosIndex(chessboard.board[my_pos + (-9 * distance)], chessboard.colors[my_pos + (-9 * distance)]);
    }

    return figuresInDiagonals;
}

std::vector<int> BishopsValuator::getBishopMovementsTab()
{
    return{ 7, 7,  7,  7,  7,  7, 7, 7,
            7, 9,  9,  9,  9,  9, 9, 7,
            7, 9, 11, 11, 11, 11, 9, 7,
            7, 9, 11, 13, 13, 11, 9, 7,
            7, 9, 11, 13, 13, 11, 9, 7,
            7, 9, 11, 11, 11, 11, 9, 7,
            7, 9,  9,  9,  9,  9, 9, 7,
            7, 7,  7,  7,  7,  7, 7, 7 };
}

//points for the presence of the pieces
vector<int> BishopsValuator::getWhiteFigPointsTab()
{
    return{ 10, 9, 5, 0, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 3, 2, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
}

vector<int> BishopsValuator::getBlackFigPointsTab()
{
    return { 0, 3, 2, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            10, 9, 5, 0, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

}

