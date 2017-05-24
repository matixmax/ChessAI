#pragma once
#include "definitions.h"
#include "Board.h"
class GameModule
{
public:
    static GameModule& i();
    void onGameAITurnament();
    void onGamePlayWithPlayer();
    void startGame(int userColor);
    void startTestMode(int userColor, int testBoardId);

private:
    void initTestBoards();
    std::pair<int,int> loadMove();
    Board makeUserMove(Board last_board);
    Board getCurrentPlayerMove(int currentPlayerColor);
    GameModule();

    Board m_lastBoard;
    std::vector<Board> m_testBoards;
    int m_userColor;
    bool m_gameExitCondition;
};

