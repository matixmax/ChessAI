#include "RocksValuator.h"
#include "FigInfo.h"

using namespace std;

namespace Valuation {

int RocksValuator::getPositionalValue(const Board & chessboard, int color, GameState phase)
{
    int value = 0; 
    value += getOneSidePositionalValue(chessboard, color, phase);
    value -= getOneSidePositionalValue(chessboard, FigInfo::not(color), phase);
    return Properies::rocksPosFact[color] * value;
}

int RocksValuator::getOneSidePositionalValue(const Board & chessboard, int color, GameState phase) {
    int value = 0;
    const int numberOfRooks = 2;
    for (int i = 0; i < numberOfRooks; i++) {
        int rook_pos = chessboard.positions[FigInfo::getPosIndex(W, color, i % 2)];
        value += getValueForRookInOpenLine(chessboard, rook_pos, color);
        value += getValueForDistanceToEnemyKing(chessboard, color, phase, rook_pos);
        value += getValueForConnectedRooksAndMobility(chessboard, rook_pos, color);
        value += getValueForRookOnSevenLine(chessboard, rook_pos, color);
    }
    return value;
}

int RocksValuator::getValueForRookOnSevenLine(const Board& chessboard, int rook_pos, int color)
{
    if (rook_pos == DESTROYED)
        return 0;

    int value = 0;
    int seventh_line[] = { 1, 6 }, eigth_line[] = { 0, 7 };
    if (rook_pos / ROW_SIZE == seventh_line[color]) {
        value += 27;
        int enemyKingPos = chessboard.positions[FigInfo::getPosIndex(K, FigInfo::not(color))];
        if (enemyKingPos / ROW_SIZE == eigth_line[color])
            value += 13;
    }
    return value;
}

int RocksValuator::getValueForDistanceToEnemyKing(const Board &chessboard, int color, GameState phase, int rookPos)
{
    if (phase != DEBUT) {
        int enemyKingPos = chessboard.positions[FigInfo::getPosIndex(K, FigInfo::not(color))];
        return 2 * proximityOfFields(rookPos, enemyKingPos);
    }
    return 0;
}

int RocksValuator::getValueForRookInOpenLine(const Board & chessboard, int rook_pos, int rook_color)
{
    if (rook_pos == DESTROYED)
        return 0;

    int value = 0;
    int pos_x = rook_pos % ROW_SIZE;
    bool open = true, self_open = true;
    for (int j = 0; j < ROW_SIZE; j++) {
        if (chessboard.board[pos_x + j * ROW_SIZE] == P + rook_color)
            self_open = false;
        if (chessboard.board[pos_x + j * ROW_SIZE] == P + FigInfo::not(rook_color))
            open = false;
    }
    if (self_open) {
        value += 4;
        if (open)
            value += 6;
    }
    return value;
}

int RocksValuator::getValueForConnectedRooksAndMobility(const Board & chessboard, int rook_pos, int rook_color)
{
    if (rook_pos == DESTROYED)
        return 0;

    int value = 0;
    int figures_in_lines[] = { DESTROYED, DESTROYED, DESTROYED, DESTROYED };
    vector<pair<int, int>> move = { {0, -1}, {1, 0}, {0, 1}, {-1, 0} };
    int movement_counter = 0;
    int pos_x = rook_pos % ROW_SIZE, pos_y = rook_pos / ROW_SIZE;
    for (int j = 0; j < 4; j++) {
        int pos[2] = { pos_x + move[j].first, pos_y + move[j].second };
        while (pos[0] >= 0 && pos[1] >= 0 && pos[0] <= 7 && pos[1] <= 7) {
            if (chessboard.board[pos[1] * ROW_SIZE + pos[0]] != EMPTY) {
                figures_in_lines[j] = FigInfo::getPosIndex(chessboard.board[pos[1] * ROW_SIZE + pos[0]], chessboard.colors[pos[1] * ROW_SIZE + pos[0]]);
                break;
            }
            movement_counter++;
            pos[0] += move[j].first;
            pos[1] += move[j].second;
        }
        if (figures_in_lines[j] != DESTROYED) {
            if (FigInfo::getColor(figures_in_lines[j]) == FigInfo::not(rook_color))
                movement_counter++;
            else {
                if (FigInfo::getFigNumber(figures_in_lines[j], rook_color) == W)
                    value += 10;
            }
        }
        value += movement_counter;
    }
    return value;
}

}

