#pragma once

#include "Definitions.h"
#include "GuiModule.h"
#include "Board.h"

class Generator{
public:
    Generator(std::vector<int8> board, std::vector<int8> positions, std::vector<int8> colors);
	Generator(Board board);
	Generator(const Generator &generator);
	
	Board GetNextPosition(int8 color);
    //bool CheckValidate(int8 current_pos, int8 move, bool &attacked, int8 distance = -1);
	bool HasNext(int8 color);
    static std::vector<Board> GetAttackMovements(const Board &chessboard, int8 color);
    static std::vector<Board> GetAvailableMovements(const Board &chessboard, int8 color, bool attack_movement = false);
    static std::vector<Board> GetPosAttackMove(const Board &chessboard, int8 target, int8 color, bool one_attack = false);
private:
	Board chessboard;
	bool attack[2];
	int8 current_figure[2];
	int8 current_move[2];
	int8 current_move_distance[2];
	int8 next_pos[2];
	int8 special_moves[2];

    static Board getBoard(const Board &chessboard, int8 color, int8 figure, int8 next_pos);
	bool pawnCondition(int8 color, int8 next_pos, bool double_move = false);
    bool validateMovement(int8 current_pos, int8 move, bool &attacked);
    static bool validateMovement(const Board &chessboard, int8 current_pos, int8 move, bool &attacked, int8 distance = -1);
    static int8 getFigureFromPosition(const Board &chessboard, int8 color, int8 figure, int8 position);
    int8 manageSpecialMoves(int8 color);
	int8 nextPos(int8 color);
    static int8 searchAttackFigure(const Board &chessboard, int8 next_pos, int8 color, int move_nb);
    static std::vector<Board> getAttackPawnMoves(const Board &chessboard, int8 color);
    static std::vector<Board> getSpecialMoves(const Board &chessboard, int8 color);
    static std::vector<int8> getFiguresOrder(const Board &chessboard, int8 color);
    static void staticDestroyAttackedFig(Board &board, int8 pos, int8 color, int8 nextPos = -1);
	void destroyAttackedFig(Board &board, int8 pos, int8 color, int8 nextPos = -1);
	void slideMovement(int8 fig, int8 fig_move_idx, bool increase_range = false);
};