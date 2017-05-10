
#include "GameModule.h"
#include "Globals.h"
#include "GuiModule.h"
#include "Initializator.h"
#include "FigInfo.h"
#include "Engine.h"
#include "SpecialsManager.h"

using namespace std;

GameModule::GameModule():m_gameExitCondition(false)
{
}

GameModule& GameModule::i()
{
    static GameModule instance;
    return instance;
}

void GameModule::onGamePlay()
{
    int8 currentPlayerColor = WHITE;
    while (m_gameExitCondition == false) {
        m_lastBoard = getCurrentPlayerMove(currentPlayerColor);
        if (m_lastBoard.states.shah == END) {
            break;
        }
        GuiModule::printBoard(m_lastBoard);
        currentPlayerColor = FigInfo::not(currentPlayerColor);
    }
}

Board GameModule::makeUserMove(Board last_board)
{
    int curr_pos, next_pos;
    do {
        cin >> curr_pos >> next_pos;
        if (SpecialsManager::i().isSpecialCode(make_pair(curr_pos, next_pos))) {
            SpecialsManager::i().getFormCode(make_pair(curr_pos, next_pos))->update();
            cin >> curr_pos >> next_pos;
        }
    } while (!Engine::userMove(curr_pos, next_pos, last_board, BLACK));
    return last_board;
}

Board GameModule::getCurrentPlayerMove(int8 currentPlayerColor)
{
    if (m_userColor == currentPlayerColor) {
        return makeUserMove(m_lastBoard);
    }
    else {
        return Engine::NormalAlfaBeta(m_lastBoard, FigInfo::not(m_userColor), 4);
    }
}

void GameModule::startGame(int8 userColor)
{
    Initializator::init();
    GuiModule::printWelcomeInfo();
    m_lastBoard = g_baseBoard;
    m_userColor = userColor;
}
