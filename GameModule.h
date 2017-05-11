#pragma once
#include "definitions.h"
#include "Board.h"
class GameModule
{
public:
    static GameModule& i();
    void onGamePlay();
    void startGame(int userColor);

private:
    std::pair<int,int> loadMove();
    Board makeUserMove(Board last_board);
    Board getCurrentPlayerMove(int currentPlayerColor);
    GameModule();

    Board m_lastBoard;
    int m_userColor;
    bool m_gameExitCondition;

    
};

