#pragma once
#include "Definitions.h"
#include "Board.h"

class Engine {
public:
	static bool userMove(int curr_pos, int next_pos, Board &chessboard, int8 color, bool trick_mode = false);
	static Board NormalAlfaBeta(Board &position, int8 color, int level);

private:
	static bool checkShah(const Board &chessboard, int8 color);
	static bool checkFiguresMovement(int curr_pos, int next_pos, const Board &chessboard);
	static int MarkPosition(const Board &Position, int8 color);
	static int MarkMaterial(const Board &position, int8 color);
	static int AlfaBetaMinimax(int level, const Board &position, int8 color, int alfa, int beta, bool max);
	static int ForcefulAlfaBeta(int level, const Board &position, int8 color, int old_material, bool max);
};