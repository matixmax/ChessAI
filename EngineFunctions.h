#pragma once
#include "Definitions.h"

#define WINDOW_EPSILON 125

bool checkFiguresMovement(int curr_pos, int next_pos, const Board &chessboard);
bool checkShah(const Board &chessboard, int8 color);
bool userMove(int curr_pos, int next_pos, Board &chessboard, int8 color, bool trick_mode = false);
int MarkPosition(const Board &Position, int8 color);
int AlfaBetaMinimax(int level, const Board &position, int8 color, int alfa, int beta, bool max);
int ForcefulAlfaBeta(int level, const Board &position, int8 color, int old_material, bool max);
Board NormalAlfaBeta(Board &position, int8 color, int level);