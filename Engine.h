#pragma once
#include "Definitions.h"
#include "Board.h"

class Engine {
public:
    static bool userMove(int curr_pos, int next_pos, Board &chessboard, int color, bool trick_mode = false);
    static Board NormalAlfaBeta(Board &position, int color, int level);

private:
    static bool checkShah(const Board &chessboard, int color);
    static bool checkFiguresMovement(int curr_pos, int next_pos, const Board &chessboard);
    static int MarkPosition(const Board &Position, int color);
    static int MarkMaterial(const Board &position, int color);
    static int AlfaBetaMinimax(int level, const Board &position, int color, int alfa, int beta, bool max);
    static int ForcefulAlfaBeta(int level, const Board &position, int color, int old_material, bool max);
};