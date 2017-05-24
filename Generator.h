#pragma once

#include "Definitions.h"
#include "GuiModule.h"
#include "Board.h"

class Generator {
public:
    Generator(std::vector<int> board, std::vector<int> positions, std::vector<int> colors);
    Generator(Board board);
    Generator(const Generator &generator);

    Board GetNextPosition(int color);
    bool HasNext(int color);
    static std::vector<Board> GetAttackMovements(const Board &chessboard, int color);
    static std::vector<Board> GetAvailableMovements(const Board &chessboard, int color, bool attack_movement = false);
    static std::vector<Board> GetOnPositionAttackMove(const Board &chessboard, int target, int color, bool one_attack = false);
    static std::vector<Board> getOnPositionPawnsAttack(const Board &chessboard, int target, int color, bool one_attack);

private:
    static std::vector<Board> getAttackPawnMoves(const Board &chessboard, int color);
    static std::vector<Board> getSpecialMoves(const Board &chessboard, int color);
    static std::vector<int> getFiguresOrder(const Board &chessboard, int color);
    static Board getBoard(const Board &chessboard, int color, int figure, int next_pos);
    static int getFigureFromPosition(const Board &chessboard, int color, int figure, int position);
    static int searchAttackFigure(const Board &chessboard, int next_pos, int color, int move_nb);
    static bool validateMovement(const Board &chessboard, int current_pos, int move, bool &attacked, int distance = -1);
    static bool isNotAKing(int color, int posIdx);
    static void staticDestroyAttackedFig(Board &board, int pos, int color, int nextPos = -1);
    int manageSpecialMoves(int color);
    int nextPos(int color);
    bool pawnCondition(int color, int next_pos, bool double_move = false);
    bool validateMovement(int current_pos, int move, bool &attacked);
    void slideMovement(int fig, int fig_move_idx, bool increase_range = false);
    void destroyAttackedFig(Board &board, int pos, int color, int nextPos = -1);
    Board m_chessboard;
    bool m_attack[2];
    int m_currentFigure[2];
    unsigned m_currentMove[2];
    int m_currentMoveDistance[2];
    int m_nextPos[2];
    int m_specialMoves[2];
};