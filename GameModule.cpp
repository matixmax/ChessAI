#include <cctype>
#include "GameModule.h"
#include "Globals.h"
#include "GuiModule.h"
#include "Initializator.h"
#include "FigInfo.h"
#include "Engine.h"
#include "SpecialsManager.h"
#include "BoardWizard.h"
#include <fstream>
#include "testBoards.h"

using namespace std;
using namespace BoardWizard;

GameModule::GameModule():m_gameExitCondition(false)
{
}



std::pair<int,int> GameModule::loadMove()
{
    string currPos, nextPos;
    cin >> currPos >> nextPos;
    if (isalpha(currPos[0]) * isdigit(currPos[1]) * isalpha(nextPos[0]) * isdigit(nextPos[1]) == 0) {
        try {
            return make_pair(stoi(currPos), stoi(nextPos));
        }
        catch (...) {
            return make_pair(c_unknownPosition, c_unknownPosition);
        }
    }
    else {
        int currPosInt = currPos[0] - 'A'+ (currPos[1] - '1') * 8 ;
        int nextPosInt = nextPos[0] - 'A'+ (nextPos[1] - '1') * 8 ;
        return make_pair(currPosInt, nextPosInt);
    }
}

GameModule& GameModule::i()
{
    static GameModule instance;
    return instance;
}

void GameModule::onGameAITurnament()
{
    fstream turnamentOutputFile;
    turnamentOutputFile.open("turnamentOutputFile.out", fstream::out);
    int currentPlayerColor = WHITE;
    while (m_gameExitCondition == false) {
        m_lastBoard = Engine::NormalAlfaBeta(m_lastBoard, currentPlayerColor, 4);
        if (m_lastBoard.states.shah == END) {
            break;
        }
        GuiModule::printBoard(m_lastBoard, turnamentOutputFile);
        GuiModule::printBoard(m_lastBoard, cout);
        currentPlayerColor = FigInfo::not(currentPlayerColor);
    }
}

void GameModule::onGamePlayWithPlayer()
{
    int currentPlayerColor = WHITE;
    while (m_gameExitCondition == false) {
        m_lastBoard = getCurrentPlayerMove(currentPlayerColor);
        if (m_lastBoard.states.shah == END) {
            break;
        }
        GuiModule::printBoard(m_lastBoard, cout);
        currentPlayerColor = FigInfo::not(currentPlayerColor);
    }
}

Board GameModule::makeUserMove(Board last_board)
{
    pair<int, int> userMove;
    do {
        userMove = loadMove();
        if (SpecialsManager::i().isSpecialCode(make_pair(userMove.first, userMove.second))) {
            SpecialsManager::i().getFormCode(make_pair(userMove.first, userMove.second))->update();
            userMove = loadMove();
        }
    } while (!Engine::userMove(userMove.first, userMove.second, last_board, BLACK));
    return last_board;
}

Board GameModule::getCurrentPlayerMove(int currentPlayerColor)
{
    if (m_userColor == currentPlayerColor) {
        return makeUserMove(m_lastBoard);
    }
    else {
        return Engine::NormalAlfaBeta(m_lastBoard, FigInfo::not(m_userColor), 4);
    }
}

void GameModule::startGame(int userColor)
{
    Initializator::init();
    GuiModule::printWelcomeInfo(std::cout);
    m_lastBoard = g_baseBoard;
    m_userColor = userColor;
}

void GameModule::startTestMode(int userColor, int testBoardId) {
    TestsBoards::initTests();
    Initializator::init();
    m_lastBoard = TestsBoards::getBoard(testBoardId);
    m_userColor = userColor;
}
