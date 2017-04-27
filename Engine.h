#pragma once
#include "Definitions.h"
#include "Board.h"

class Engine {
public:
	bool userMove(int curr_pos, int next_pos, Board &chessboard, int8 color, bool trick_mode = false);
	Board NormalAlfaBeta(Board &position, int8 color, int level);

private:
	bool checkShah(const Board &chessboard, int8 color);
	bool checkFiguresMovement(int curr_pos, int next_pos, const Board &chessboard);
	int MarkPosition(const Board &Position, int8 color);
	int MarkMaterial(const Board &position, int8 color);
	int AlfaBetaMinimax(int level, const Board &position, int8 color, int alfa, int beta, bool max);
	int ForcefulAlfaBeta(int level, const Board &position, int8 color, int old_material, bool max);
};