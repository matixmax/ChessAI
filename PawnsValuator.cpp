#include "PawnsValuator.h"
#include "FigInfo.h"

using namespace std;

namespace Valuation {

int PawnsValuator::getOneSidePositionalValue(const Board & chessboard, int color, GameState phase)
{
    int value = 0;
    for (int pawn = 0; pawn < 8; pawn++) {
        int my_pos = chessboard.positions[FigInfo::getPosIndex(P, color, pawn)];

        value -= countValueForIzolation(chessboard, color, my_pos);
        value -= countValueForMirroring(chessboard, color, my_pos);
        value += countValueForProximityToCenter(my_pos, phase);
        value += countValueForComingPawns(chessboard, color, my_pos, phase);
    }
    return value;
}

int PawnsValuator::getPositionalValue(const Board & chessboard, int color, GameState phase)
{
    int value = 0;
    value += getOneSidePositionalValue(chessboard, color, phase);
    value -= getOneSidePositionalValue(chessboard, FigInfo::not(color), phase);
    return Properies::pawnsPosFact[color] * value;
}

int PawnsValuator::countValueForIzolation(const Board & chessboard, int color, int pos)
{
    if (pos != DESTROYED && izolatedPawn(chessboard, color, pos))
        return 20;
    else
        return 0;
}

int PawnsValuator::countValueForMirroring(const Board & chessboard, int color, int pos)
{
    if (pos != DESTROYED && mirroredPawn(chessboard, color, pos))
        return 10;
    else
        return 0;
}

bool PawnsValuator::izolatedPawn(const Board & chessboard, int color, int pos) {
    int neighborhood[2];
    int my_fig;
    if (color == WHITE) { my_fig = Pb; neighborhood[0] = 9; neighborhood[1] = 7; }
    else { my_fig = Pc; neighborhood[0] = -7; neighborhood[1] = -9; }
    if (pos % ROW_SIZE != 0 && chessboard.board[pos + neighborhood[1]] == my_fig)
        return false;
    if (pos % ROW_SIZE != 7 && chessboard.board[pos + neighborhood[0]] == my_fig)
        return false;
    return true;
}

bool PawnsValuator::mirroredPawn(const Board & chessboard, int color, int pos) {
    int y = pos % ROW_SIZE;
    int my_fig = P + color;
    for (int i = 0; i < ROW_SIZE; i++) {
        if (chessboard.board[y + i * ROW_SIZE] == my_fig && y + i * ROW_SIZE != pos)
            return true;
    }
    return false;
}

int PawnsValuator::countValueForProximityToCenter(int pos, GameState phase)
{
    if (phase == DEBUT || phase == MID_GAME) {
        int proximity = proximityToTheCenter(pos);

        switch (proximity) 
        {
        case 6:
            return proximity + 15;
        case 5:
            return proximity + 10;
        default:
            return proximity;
        }
    }
    return 0;
}

int PawnsValuator::countValueForComingPawns(const Board & chessboard, int color, int pos, GameState phase)
{
    if (pos == DESTROYED)
        return 0;

    if (phase == EARLY_ENDING || phase == ENDING) {
        int factor = 2;
        if (phase == ENDING)
            factor += 2;
        int pos_y = pos / ROW_SIZE;
        if (color == WHITE) {
            return factor * (6 - pos_y) * (6 - pos_y);
        }
        else {
            return factor * (pos_y - 1) * (pos_y - 1);
        }
    }

    return 0;
}

}
